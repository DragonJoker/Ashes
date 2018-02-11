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
#include <Pipeline/InputAssemblyState.hpp>
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
#include <RenderPass/FrameBufferAttachment.hpp>
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

		std::vector< renderer::PixelFormat > doGetFormats( renderer::TextureView const & depthView
			, renderer::TextureView const & colourView )
		{
			return
			{
				depthView.getFormat(),
				colourView.getFormat(),
			};
		}

		renderer::RenderPassAttachmentArray doGetAttaches( renderer::TextureView const & depthView
			, renderer::TextureView const & colourView )
		{
			return renderer::RenderPassAttachmentArray
			{
				{ depthView.getFormat(), false },
				{ colourView.getFormat(), true }
			};
		}

		renderer::RenderPassPtr doCreateRenderPass( renderer::Device const & device
			, renderer::TextureView const & depthView
			, renderer::TextureView const & colourView )
		{
			auto formats = doGetFormats( depthView, colourView );
			auto attaches = doGetAttaches( depthView, colourView );
			renderer::ImageLayoutArray const initialLayouts
			{
				renderer::ImageLayout::eDepthStencilAttachmentOptimal,
				renderer::ImageLayout::eUndefined,
			};
			renderer::ImageLayoutArray const finalLayouts
			{
				renderer::ImageLayout::eDepthStencilAttachmentOptimal,
				renderer::ImageLayout::eColourAttachmentOptimal,
			};
			renderer::RenderSubpassPtrArray subpasses;
			subpasses.emplace_back( device.createRenderSubpass( formats
				, { renderer::PipelineStageFlag::eColourAttachmentOutput, renderer::AccessFlag::eColourAttachmentWrite } ) );
			return device.createRenderPass( attaches
				, subpasses
				, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
					, renderer::AccessFlag::eColourAttachmentWrite
					, initialLayouts }
				, renderer::RenderPassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
					, renderer::AccessFlag::eColourAttachmentWrite
					, finalLayouts } );
		}

		renderer::FrameBufferPtr doCreateFrameBuffer( renderer::RenderPass const & renderPass
			, renderer::TextureView const & depthView
			, renderer::TextureView const & colourView )
		{
			auto formats = doGetFormats( depthView, colourView );
			renderer::FrameBufferAttachmentArray attaches;
			attaches.emplace_back( *( renderPass.begin() + 0u ), depthView );
			attaches.emplace_back( *( renderPass.begin() + 1u ), colourView );
			auto dimensions = colourView.getTexture().getDimensions();
			return renderPass.createFrameBuffer( renderer::UIVec2{ dimensions[0], dimensions[1] }
				, std::move( attaches ) );
		}

		renderer::DescriptorSetLayoutPtr doCreateGBufferDescriptorLayout( renderer::Device const & device )
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

		renderer::DescriptorSetLayoutPtr doCreateUboDescriptorLayout( renderer::Device const & device )
		{
			std::vector< renderer::DescriptorSetLayoutBinding > bindings
			{
				renderer::DescriptorSetLayoutBinding{ 0u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eFragment },
				renderer::DescriptorSetLayoutBinding{ 1u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eFragment },
			};
			return device.createDescriptorSetLayout( std::move( bindings ) );
		}

		renderer::DescriptorSetPtr doCreateUboDescriptorSet( renderer::DescriptorSetPool const & pool
			, renderer::UniformBuffer< common::LightsData > const & lightsUbo
			, renderer::UniformBuffer< renderer::Mat4 > const & matrixUbo )
		{
			auto & layout = pool.getLayout();
			auto result = pool.createDescriptorSet( 1u );
			result->createBinding( layout.getBinding( 0u )
				, lightsUbo
				, 0u
				, 1u );
			result->createBinding( layout.getBinding( 1u )
				, matrixUbo
				, 0u
				, 1u );
			result->update();
			return result;
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
		, renderer::UniformBuffer< common::LightsData > const & lightsUbo
		, renderer::StagingBuffer & stagingBuffer
		, renderer::TextureView const & depthView
		, renderer::TextureView const & colourView )
		: m_device{ device }
		, m_lightsUbo{ lightsUbo }
		, m_updateCommandBuffer{ m_device.getGraphicsCommandPool().createCommandBuffer() }
		, m_commandBuffer{ m_device.getGraphicsCommandPool().createCommandBuffer() }
		, m_matrixUbo{ renderer::makeUniformBuffer< renderer::Mat4 >( device, 1u, renderer::BufferTarget::eTransferDst, renderer::MemoryPropertyFlag::eDeviceLocal ) }
		, m_gbufferDescriptorLayout{ doCreateGBufferDescriptorLayout( m_device ) }
		, m_gbufferDescriptorPool{ m_gbufferDescriptorLayout->createPool( 1u, false ) }
		, m_uboDescriptorLayout{ doCreateUboDescriptorLayout( m_device ) }
		, m_uboDescriptorPool{ m_uboDescriptorLayout->createPool( 1u ) }
		, m_uboDescriptorSet{ doCreateUboDescriptorSet( *m_uboDescriptorPool, m_lightsUbo, *m_matrixUbo ) }
		, m_program{ doCreateProgram( m_device ) }
		, m_renderPass{ doCreateRenderPass( m_device, depthView, colourView ) }
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
		, m_pipelineLayout{ m_device.createPipelineLayout( { *m_gbufferDescriptorLayout, *m_uboDescriptorLayout } ) }
		, m_pipeline{ m_pipelineLayout->createPipeline( *m_program
			, { *m_vertexLayout }
			, *m_renderPass
			, { renderer::PrimitiveTopology::eTriangleStrip } ) }
	{
		m_pipeline->multisampleState( renderer::MultisampleState{} );
		m_pipeline->depthStencilState( renderer::DepthStencilState
		{
			0u,
			false,
			false,
			renderer::CompareOp::eLess
		} );
		m_pipeline->finish();
	}

	void LightingPass::update( renderer::Mat4 const & viewProj
		, renderer::StagingBuffer & stagingBuffer
		, renderer::TextureView const & colourView
		, renderer::TextureView const & depthView
		, GeometryPassResult const & geometryBuffers )
	{
		m_geometryBuffers = &geometryBuffers;
		m_colourView = &colourView;
		m_depthView = &depthView;

		m_matrixUbo->getData( 0u ) = utils::inverse( viewProj );
		stagingBuffer.uploadUniformData( *m_updateCommandBuffer
			, m_matrixUbo->getDatas()
			, *m_matrixUbo
			, renderer::PipelineStageFlag::eFragmentShader );

		auto dimensions = depthView.getTexture().getDimensions();
		auto size = renderer::UIVec2{ dimensions[0], dimensions[1] };
		m_frameBuffer = doCreateFrameBuffer( *m_renderPass, depthView, colourView );
		m_gbufferDescriptorSet.reset();
		m_gbufferDescriptorSet = m_gbufferDescriptorPool->createDescriptorSet( 0u );
		auto & result = *m_geometryBuffers;

		for ( size_t i = 0; i < result.size(); ++i )
		{
			m_gbufferDescriptorSet->createBinding( m_gbufferDescriptorLayout->getBinding( i )
				, *result[i].view
				, *m_sampler );
		}

		m_gbufferDescriptorSet->update();
		m_commandBuffer->reset();
		auto & commandBuffer = *m_commandBuffer;
		static renderer::DepthStencilClearValue const depth{ 1.0, 0 };
		static renderer::RgbaColour const colour{ 1.0f, 0.8f, 0.4f, 0.0f };

		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eSimultaneousUse ) )
		{
			commandBuffer.beginRenderPass( *m_renderPass
				, *m_frameBuffer
				, { depth, colour }
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
			commandBuffer.bindDescriptorSet( *m_gbufferDescriptorSet
				, *m_pipelineLayout );
			commandBuffer.bindDescriptorSet( *m_uboDescriptorSet
				, *m_pipelineLayout );
			commandBuffer.draw( 4u );
			commandBuffer.endRenderPass();
			commandBuffer.end();
		}
	}

	bool LightingPass::draw()const
	{
		return m_device.getGraphicsQueue().submit( *m_commandBuffer, nullptr );
	}
}
