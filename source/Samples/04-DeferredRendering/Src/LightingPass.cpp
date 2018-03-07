#include "LightingPass.hpp"

#include <Buffer/Buffer.hpp>
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
#include <Miscellaneous/QueryPool.hpp>
#include <Pipeline/DepthStencilState.hpp>
#include <Pipeline/InputAssemblyState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/PipelineLayout.hpp>
#include <Pipeline/Scissor.hpp>
#include <Pipeline/VertexLayout.hpp>
#include <Pipeline/Viewport.hpp>
#include <RenderPass/RenderPass.hpp>
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
		std::vector< renderer::ShaderStageState > doCreateProgram( renderer::Device const & device )
		{
			std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";

			if ( !wxFileExists( shadersFolder / "opaque_lp.vert" )
				|| !wxFileExists( shadersFolder / "opaque_lp.frag" ) )
			{
				throw std::runtime_error{ "Shader files are missing" };
			}

			std::vector< renderer::ShaderStageState > shaderStages;
			shaderStages.push_back( { device.createShaderModule( renderer::ShaderStageFlag::eVertex ) } );
			shaderStages.push_back( { device.createShaderModule( renderer::ShaderStageFlag::eFragment ) } );
			shaderStages[0].module->loadShader( common::dumpTextFile( shadersFolder / "opaque_lp.vert" ) );
			shaderStages[1].module->loadShader( common::dumpTextFile( shadersFolder / "opaque_lp.frag" ) );
			return shaderStages;
		}

		std::vector< renderer::Format > doGetFormats( renderer::TextureView const & depthView
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
				{
					0u,
					depthView.getFormat(),
					renderer::SampleCountFlag::e1,
					renderer::AttachmentLoadOp::eLoad,
					renderer::AttachmentStoreOp::eStore,
					renderer::AttachmentLoadOp::eLoad,
					renderer::AttachmentStoreOp::eStore,
					renderer::ImageLayout::eUndefined,
					renderer::ImageLayout::eDepthStencilAttachmentOptimal,
				},
				{
					1u,
					colourView.getFormat(),
					renderer::SampleCountFlag::e1,
					renderer::AttachmentLoadOp::eClear,
					renderer::AttachmentStoreOp::eStore,
					renderer::AttachmentLoadOp::eDontCare,
					renderer::AttachmentStoreOp::eDontCare,
					renderer::ImageLayout::eUndefined,
					renderer::ImageLayout::eDepthStencilAttachmentOptimal,
				}
			};
		}

		renderer::RenderPassPtr doCreateRenderPass( renderer::Device const & device
			, renderer::TextureView const & depthView
			, renderer::TextureView const & colourView )
		{
			renderer::RenderSubpassAttachmentArray subAttaches
			{
				renderer::RenderSubpassAttachment{ 1u, renderer::ImageLayout::eColourAttachmentOptimal },
			};
			renderer::RenderSubpassPtrArray subpasses;
			subpasses.emplace_back( device.createRenderSubpass( renderer::PipelineBindPoint::eGraphics
				, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eColourAttachmentOutput, renderer::AccessFlag::eColourAttachmentWrite }
				, subAttaches
				, renderer::RenderSubpassAttachment{ 0u, renderer::ImageLayout::eDepthStencilAttachmentOptimal } ) );
			return device.createRenderPass( doGetAttaches( depthView, colourView )
				, std::move( subpasses )
				, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
					, renderer::AccessFlag::eColourAttachmentWrite }
				, renderer::RenderSubpassState{ renderer::PipelineStageFlag::eColourAttachmentOutput
					, renderer::AccessFlag::eColourAttachmentWrite } );
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
			return renderPass.createFrameBuffer( renderer::Extent2D{ dimensions.width, dimensions.height }
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
			, renderer::UniformBuffer< common::SceneData > const & sceneUbo )
		{
			auto & layout = pool.getLayout();
			auto result = pool.createDescriptorSet( 1u );
			result->createBinding( layout.getBinding( 0u )
				, lightsUbo
				, 0u
				, 1u );
			result->createBinding( layout.getBinding( 1u )
				, sceneUbo
				, 0u
				, 1u );
			result->update();
			return result;
		}

		renderer::VertexBufferPtr< common::TexturedVertexData > doCreateVertexBuffer( renderer::Device const & device
			, renderer::StagingBuffer & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer )
		{
			std::vector< common::TexturedVertexData > vertexData
			{
				{ { -1.0, -1.0, 0.0, 1.0 }, { 0.0, 0.0 } },
				{ { -1.0, +1.0, 0.0, 1.0 }, { 0.0, 1.0 } },
				{ { +1.0, -1.0, 0.0, 1.0 }, { 1.0, 0.0 } },
				{ { +1.0, +1.0, 0.0, 1.0 }, { 1.0, 1.0 } },
			};
			auto result = renderer::makeVertexBuffer< common::TexturedVertexData >( device
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
			auto result = renderer::makeLayout< common::TexturedVertexData >( 0 );
			result->createAttribute( 0u
				, renderer::Format::eR32G32B32A32_SFLOAT
				, uint32_t( offsetof( common::TexturedVertexData, position ) ) );
			result->createAttribute( 1u
				, renderer::Format::eR32G32_SFLOAT
				, uint32_t( offsetof( common::TexturedVertexData, uv ) ) );
			return result;
		}
	}

	LightingPass::LightingPass( renderer::Device const & device
		, renderer::UniformBuffer< common::LightsData > const & lightsUbo
		, renderer::StagingBuffer & stagingBuffer
		, renderer::TextureViewCRefArray const & views )
		: m_device{ device }
		, m_lightsUbo{ lightsUbo }
		, m_updateCommandBuffer{ m_device.getGraphicsCommandPool().createCommandBuffer() }
		, m_commandBuffer{ m_device.getGraphicsCommandPool().createCommandBuffer() }
		, m_sceneUbo{ renderer::makeUniformBuffer< common::SceneData >( device, 1u, renderer::BufferTarget::eTransferDst, renderer::MemoryPropertyFlag::eDeviceLocal ) }
		, m_gbufferDescriptorLayout{ doCreateGBufferDescriptorLayout( m_device ) }
		, m_gbufferDescriptorPool{ m_gbufferDescriptorLayout->createPool( 1u, false ) }
		, m_uboDescriptorLayout{ doCreateUboDescriptorLayout( m_device ) }
		, m_uboDescriptorPool{ m_uboDescriptorLayout->createPool( 1u ) }
		, m_uboDescriptorSet{ doCreateUboDescriptorSet( *m_uboDescriptorPool, m_lightsUbo, *m_sceneUbo ) }
		, m_renderPass{ doCreateRenderPass( m_device, views[0].get(), views[1].get() ) }
		, m_sampler{ m_device.createSampler( renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::WrapMode::eClampToEdge
			, renderer::Filter::eNearest
			, renderer::Filter::eNearest ) }
		, m_vertexBuffer{ doCreateVertexBuffer( m_device, stagingBuffer, *m_updateCommandBuffer ) }
		, m_vertexLayout{ doCreateVertexLayout( m_device ) }
		, m_pipelineLayout{ m_device.createPipelineLayout( { *m_gbufferDescriptorLayout, *m_uboDescriptorLayout } ) }
		, m_pipeline{ m_pipelineLayout->createPipeline( 
			{
				doCreateProgram( m_device ),
				*m_renderPass,
				renderer::VertexInputState::create( *m_vertexLayout ),
				{ renderer::PrimitiveTopology::eTriangleStrip },
				renderer::RasterisationState{},
				renderer::MultisampleState{},
				renderer::ColourBlendState::createDefault(),
				{ renderer::DynamicState::eViewport, renderer::DynamicState::eScissor },
				renderer::DepthStencilState{ 0u, false, false, renderer::CompareOp::eLess }
			} )
		}
		, m_queryPool{ m_device.createQueryPool( renderer::QueryType::eTimestamp, 2u, 0u ) }
	{
	}

	void LightingPass::update( common::SceneData const & sceneData
		, renderer::StagingBuffer & stagingBuffer
		, renderer::TextureViewCRefArray const & views
		, GeometryPassResult const & geometryBuffers )
	{
		m_geometryBuffers = &geometryBuffers;
		m_depthView = &views[0].get();
		m_colourView = &views[1].get();

		m_sceneUbo->getData( 0u ).mtxProjection = utils::inverse( sceneData.mtxProjection );
		stagingBuffer.uploadUniformData( *m_updateCommandBuffer
			, m_sceneUbo->getDatas()
			, *m_sceneUbo
			, renderer::PipelineStageFlag::eFragmentShader );

		auto dimensions = m_depthView->getTexture().getDimensions();
		auto size = renderer::UIVec2{ dimensions.width, dimensions.height };
		m_frameBuffer = doCreateFrameBuffer( *m_renderPass, *m_depthView, *m_colourView );
		m_gbufferDescriptorSet.reset();
		m_gbufferDescriptorSet = m_gbufferDescriptorPool->createDescriptorSet( 0u );
		auto & gbuffer = *m_geometryBuffers;

		for ( size_t i = 0; i < gbuffer.size(); ++i )
		{
			m_gbufferDescriptorSet->createBinding( m_gbufferDescriptorLayout->getBinding( i )
				, *gbuffer[i].view
				, *m_sampler );
		}

		m_gbufferDescriptorSet->update();
		m_commandBuffer->reset();
		auto & commandBuffer = *m_commandBuffer;
		static renderer::DepthStencilClearValue const depth{ 1.0, 0 };
		static renderer::RgbaColour const colour{ 1.0f, 0.8f, 0.4f, 0.0f };

		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eSimultaneousUse ) )
		{
			commandBuffer.resetQueryPool( *m_queryPool, 0u, 2u );
			commandBuffer.writeTimestamp( renderer::PipelineStageFlag::eTopOfPipe
				, *m_queryPool
				, 0u );

			for ( auto & texture : gbuffer )
			{
				commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eColourAttachmentOutput
					, renderer::PipelineStageFlag::eFragmentShader
					, texture.view->makeShaderInputResource( renderer::ImageLayout::eColourAttachmentOptimal
						, renderer::AccessFlag::eColourAttachmentWrite ) );
			}

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
			commandBuffer.bindVertexBuffer( 0u, m_vertexBuffer->getBuffer(), 0u );
			commandBuffer.bindDescriptorSet( *m_gbufferDescriptorSet
				, *m_pipelineLayout );
			commandBuffer.bindDescriptorSet( *m_uboDescriptorSet
				, *m_pipelineLayout );
			commandBuffer.draw( 4u );
			commandBuffer.endRenderPass();
			commandBuffer.writeTimestamp( renderer::PipelineStageFlag::eTopOfPipe
				, *m_queryPool
				, 1u );
			commandBuffer.end();
		}
	}

	bool LightingPass::draw( std::chrono::nanoseconds & gpu )const
	{
		bool result = m_device.getGraphicsQueue().submit( *m_commandBuffer, nullptr );

		if ( result )
		{
			renderer::UInt32Array values{ 0u, 0u };
			m_queryPool->getResults( 0u
				, 2u
				, 0u
				, renderer::QueryResultFlag::eWait
				, values );
			gpu += std::chrono::nanoseconds{ uint64_t( ( values[1] - values[0] ) / float( m_device.getTimestampPeriod() ) ) };
		}

		return result;
	}
}
