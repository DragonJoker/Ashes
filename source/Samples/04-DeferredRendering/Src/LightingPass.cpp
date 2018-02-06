#include "LightingPass.hpp"

#include <Buffer/Buffer.hpp>
#include <Buffer/GeometryBuffers.hpp>
#include <Buffer/StagingBuffer.hpp>
#include <Buffer/VertexBuffer.hpp>
#include <Command/CommandBuffer.hpp>
#include <Command/CommandPool.hpp>
#include <Core/Device.hpp>
#include <Descriptor/DescriptorSet.hpp>
#include <Descriptor/DescriptorSetLayout.hpp>
#include <Descriptor/DescriptorSetLayoutBinding.hpp>
#include <Descriptor/DescriptorSetPool.hpp>
#include <Image/Texture.hpp>
#include <Image/TextureView.hpp>
#include <Pipeline/DepthStencilState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Pipeline/Scissor.hpp>
#include <Pipeline/VertexLayout.hpp>
#include <Pipeline/Viewport.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderPassState.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <RenderPass/TextureAttachment.hpp>
#include <Shader/ShaderProgram.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

#include <FileUtils.hpp>

namespace vkapp
{
	namespace
	{
		renderer::ShaderProgramPtr doCreateProgram( renderer::Device const & device )
		{
			renderer::ShaderProgramPtr result = device.createShaderProgram();
			std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";

			if ( !wxFileExists( shadersFolder / "opaque_lp.vert" )
				|| !wxFileExists( shadersFolder / "opaque_lp.frag" ) )
			{
				throw std::runtime_error{ "Shader files are missing" };
			}

			result->createModule( common::dumpTextFile( shadersFolder / "opaque_lp.vert" )
				, renderer::ShaderStageFlag::eVertex );
			result->createModule( common::dumpTextFile( shadersFolder / "opaque_lp.frag" )
				, renderer::ShaderStageFlag::eFragment );
			return result;
		}

		std::vector< renderer::PixelFormat > doGetFormats( renderer::TextureView const & colourView )
		{
			return
			{
				colourView.getFormat()
			};
		}
		renderer::RenderPassPtr doCreateRenderPass( renderer::Device const & device
			, renderer::TextureView const & colourView )
		{
			auto formats = doGetFormats( colourView );
			renderer::RenderSubpassPtrArray subpasses;
			subpasses.emplace_back( device.createRenderSubpass( formats
				, { renderer::PipelineStageFlag::eColourAttachmentOutput, renderer::AccessFlag::eColourAttachmentWrite } ) );
			return device.createRenderPass( formats
				, subpasses
				, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
					, renderer::AccessFlag::eColourAttachmentWrite
					, { renderer::ImageLayout::eColourAttachmentOptimal } }
				, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
					, renderer::AccessFlag::eColourAttachmentWrite
					, { renderer::ImageLayout::eColourAttachmentOptimal } } );
		}

		renderer::FrameBufferPtr doCreateFrameBuffer( renderer::RenderPass const & renderPass
			, renderer::TextureView const & colourView )
		{
			auto formats = doGetFormats( colourView );
			renderer::TextureAttachmentPtrArray attaches;
			attaches.emplace_back( std::make_unique< renderer::TextureAttachment >( colourView ) );
			auto dimensions = colourView.getTexture().getDimensions();
			return renderPass.createFrameBuffer( renderer::UIVec2{ dimensions[0], dimensions[1] }
				, std::move( attaches ) );
		}

		renderer::DescriptorSetLayoutPtr doCreateDescriptorLayout( renderer::Device const & device )
		{
			std::vector< renderer::DescriptorSetLayoutBinding > bindings
			{
				renderer::DescriptorSetLayoutBinding{ 0u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment },
				renderer::DescriptorSetLayoutBinding{ 1u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment },
				renderer::DescriptorSetLayoutBinding{ 2u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment },
				renderer::DescriptorSetLayoutBinding{ 3u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment },
				renderer::DescriptorSetLayoutBinding{ 4u, renderer::DescriptorType::eCombinedImageSampler, renderer::ShaderStageFlag::eFragment },
			};
			return device.createDescriptorSetLayout( std::move( bindings ) );
		}

		renderer::VertexBufferPtr< TexturedVertexData > doCreateVertexBuffer( renderer::Device const & device
			, renderer::StagingBuffer & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer )
		{
			std::vector< TexturedVertexData > vertexData
			{
				{ { -1.0, -1.0, 0.0, 1.0 }, { 0.0, 0.0 } },
				{ { -1.0, +1.0, 0.0, 1.0 }, { 0.0, 1.0 } },
				{ { +1.0, -1.0, 0.0, 1.0 }, { 1.0, 0.0 } },
				{ { +1.0, +1.0, 0.0, 1.0 }, { 1.0, 1.0 } },
			};
			auto result = renderer::makeVertexBuffer< TexturedVertexData >( device
				, uint32_t( vertexData.size() )
				, renderer::BufferTarget::eTransferDst
				, renderer::MemoryPropertyFlag::eDeviceLocal );
			stagingBuffer.uploadVertexData( commandBuffer
				, vertexData
				, *result
				, renderer::PipelineStageFlag::eVertexInput );
			return result;
		}

		renderer::VertexLayoutPtr doCreateVertexLayout( renderer::Device const & device )
		{
			auto result = renderer::makeLayout< TexturedVertexData >( device, 0 );
			result->createAttribute< renderer::Vec4 >( 0u
				, uint32_t( offsetof( TexturedVertexData, position ) ) );
			result->createAttribute< renderer::Vec2 >( 1u
				, uint32_t( offsetof( TexturedVertexData, uv ) ) );
			return result;
		}
	}

	LightingPass::LightingPass( renderer::Device const & device
		, renderer::UniformBuffer< renderer::Mat4 > const & matrixUbo
		, renderer::UniformBuffer< common::LightsData > const & lightsUbo
		, renderer::StagingBuffer & stagingBuffer
		, renderer::TextureView const & colourView )
		: m_device{ device }
		, m_matrixUbo{ matrixUbo }
		, m_lightsUbo{ lightsUbo }
		, m_updateCommandBuffer{ m_device.getGraphicsCommandPool().createCommandBuffer() }
		, m_commandBuffer{ m_device.getGraphicsCommandPool().createCommandBuffer() }
		, m_descriptorLayout{ doCreateDescriptorLayout( m_device ) }
		, m_descriptorPool{ m_descriptorLayout->createPool( 1u, false ) }
		, m_program{ doCreateProgram( m_device ) }
		, m_renderPass{ doCreateRenderPass( m_device, colourView ) }
		, m_sampler{ m_device.createSampler( renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::Filter::eNearest
			, renderer::Filter::eNearest ) }
		, m_vertexBuffer{ doCreateVertexBuffer( m_device, stagingBuffer, *m_updateCommandBuffer ) }
		, m_vertexLayout{ doCreateVertexLayout( m_device ) }
		, m_vao{ m_device.createGeometryBuffers( *m_vertexBuffer
			, 0u
			, *m_vertexLayout ) }
		, m_pipelineLayout{ m_device.createPipelineLayout( *m_descriptorLayout ) }
		, m_pipeline{ m_pipelineLayout->createPipeline( *m_program
			, { *m_vertexLayout }
			, *m_renderPass
			, renderer::PrimitiveTopology::eTriangleStrip ) }
	{
		m_pipeline->multisampleState( renderer::MultisampleState{} );
		m_pipeline->depthStencilState( renderer::DepthStencilState{} );
		m_pipeline->finish();
	}

	void LightingPass::update( renderer::TextureView const & colourView
		, renderer::TextureView const & depthView
		, GeometryPassResult const & geometryBuffers )
	{
		m_geometryBuffers = &geometryBuffers;
		m_colourView = &colourView;
		m_depthView = &depthView;
		auto dimensions = depthView.getTexture().getDimensions();
		auto size = renderer::UIVec2{ dimensions[0], dimensions[1] };
		m_frameBuffer = doCreateFrameBuffer( *m_renderPass, colourView );
		m_descriptorSet.reset();
		m_descriptorSet = m_descriptorPool->createDescriptorSet();
		auto & result = *m_geometryBuffers;
		m_descriptorSet->createBinding( m_descriptorLayout->getBinding( 0u )
			, *m_depthView
			, *m_sampler );
		m_descriptorSet->createBinding( m_descriptorLayout->getBinding( 1u )
			, *result[0].view
			, *m_sampler );
		m_descriptorSet->createBinding( m_descriptorLayout->getBinding( 2u )
			, *result[1].view
			, *m_sampler );
		m_descriptorSet->createBinding( m_descriptorLayout->getBinding( 3u )
			, *result[2].view
			, *m_sampler );
		m_descriptorSet->createBinding( m_descriptorLayout->getBinding( 4u )
			, *result[3].view
			, *m_sampler );
		m_descriptorSet->update();
		m_commandBuffer->reset();
		auto & commandBuffer = *m_commandBuffer;
		static renderer::RgbaColour const colour{ 1.0f, 0.8f, 0.4f, 0.0f };

		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eSimultaneousUse ) )
		{
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eFragmentShader
				, renderer::PipelineStageFlag::eColourAttachmentOutput
				, colourView.makeColourAttachment() );
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::PipelineStageFlag::eFragmentShader
				, depthView.makeShaderInputResource() );
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::PipelineStageFlag::eFragmentShader
				, result[0].view->makeShaderInputResource() );
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::PipelineStageFlag::eFragmentShader
				, result[1].view->makeShaderInputResource() );
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::PipelineStageFlag::eFragmentShader
				, result[2].view->makeShaderInputResource() );
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
				, renderer::PipelineStageFlag::eFragmentShader
				, result[3].view->makeShaderInputResource() );
			commandBuffer.beginRenderPass( *m_renderPass
				, *m_frameBuffer
				, { colour }
			, renderer::SubpassContents::eInline );
			commandBuffer.bindPipeline( *m_pipeline );
			commandBuffer.setViewport( { size[0]
				, size[1]
				, 0
				, 0 } );
			commandBuffer.setScissor( { 0
				, 0
				, size[0]
				, size[1] } );
			commandBuffer.bindGeometryBuffers( *m_vao );
			commandBuffer.bindDescriptorSet( *m_descriptorSet
				, *m_pipelineLayout );
			commandBuffer.draw( 4u
				, 1u
				, 0u
				, 0u );
			commandBuffer.endRenderPass();
			commandBuffer.end();
		}
	}

	void LightingPass::draw()const
	{
		m_device.getGraphicsQueue().submit( *m_commandBuffer, nullptr );
	}
}
