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
#include <Image/Sampler.hpp>
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
#include <RenderPass/FrameBuffer.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>
#include <RenderPass/FrameBufferAttachment.hpp>
#include <GlslToSpv.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

#include <FileUtils.hpp>

namespace vkapp
{
	namespace
	{
		std::vector< ashes::ShaderStageState > doCreateProgram( ashes::Device const & device )
		{
			std::string shadersFolder = common::getPath( common::getExecutableDirectory() ) / "share" / AppName / "Shaders";

			if ( !wxFileExists( shadersFolder / "opaque_lp.vert" )
				|| !wxFileExists( shadersFolder / "opaque_lp.frag" ) )
			{
				throw std::runtime_error{ "Shader files are missing" };
			}

			std::vector< ashes::ShaderStageState > shaderStages;
			shaderStages.push_back( { device.createShaderModule( ashes::ShaderStageFlag::eVertex ) } );
			shaderStages.push_back( { device.createShaderModule( ashes::ShaderStageFlag::eFragment ) } );
			shaderStages[0].module->loadShader( common::dumpShaderFile( device, ashes::ShaderStageFlag::eVertex, shadersFolder / "opaque_lp.vert" ) );
			shaderStages[1].module->loadShader( common::dumpShaderFile( device, ashes::ShaderStageFlag::eFragment, shadersFolder / "opaque_lp.frag" ) );
			return shaderStages;
		}

		std::vector< ashes::Format > doGetFormats( ashes::TextureView const & depthView
			, ashes::TextureView const & colourView )
		{
			return
			{
				depthView.getFormat(),
				colourView.getFormat(),
			};
		}

		ashes::AttachmentDescriptionArray doGetAttaches( ashes::TextureView const & depthView
			, ashes::TextureView const & colourView )
		{
			return ashes::AttachmentDescriptionArray
			{
				{
					depthView.getFormat(),
					ashes::SampleCountFlag::e1,
					ashes::AttachmentLoadOp::eLoad,
					ashes::AttachmentStoreOp::eStore,
					ashes::AttachmentLoadOp::eLoad,
					ashes::AttachmentStoreOp::eStore,
					ashes::ImageLayout::eUndefined,
					ashes::ImageLayout::eDepthStencilAttachmentOptimal,
				},
				{
					colourView.getFormat(),
					ashes::SampleCountFlag::e1,
					ashes::AttachmentLoadOp::eClear,
					ashes::AttachmentStoreOp::eStore,
					ashes::AttachmentLoadOp::eDontCare,
					ashes::AttachmentStoreOp::eDontCare,
					ashes::ImageLayout::eUndefined,
					ashes::ImageLayout::eDepthStencilAttachmentOptimal,
				}
			};
		}

		ashes::RenderPassPtr doCreateRenderPass( ashes::Device const & device
			, ashes::TextureView const & depthView
			, ashes::TextureView const & colourView )
		{
			ashes::AttachmentReferenceArray subAttaches
			{
				ashes::AttachmentReference{ 1u, ashes::ImageLayout::eColourAttachmentOptimal },
			};
			ashes::RenderSubpassPtrArray subpasses;
			subpasses.emplace_back( std::make_unique< ashes::RenderSubpass >( ashes::PipelineBindPoint::eGraphics
				, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eColourAttachmentOutput, ashes::AccessFlag::eColourAttachmentWrite }
				, subAttaches
				, ashes::AttachmentReference{ 0u, ashes::ImageLayout::eDepthStencilAttachmentOptimal } ) );
			return device.createRenderPass( doGetAttaches( depthView, colourView )
				, std::move( subpasses )
				, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eColourAttachmentOutput
					, ashes::AccessFlag::eColourAttachmentWrite }
				, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eColourAttachmentOutput
					, ashes::AccessFlag::eColourAttachmentWrite } );
		}

		ashes::FrameBufferPtr doCreateFrameBuffer( ashes::RenderPass const & renderPass
			, ashes::TextureView const & depthView
			, ashes::TextureView const & colourView )
		{
			auto formats = doGetFormats( depthView, colourView );
			ashes::FrameBufferAttachmentArray attaches;
			attaches.emplace_back( *( renderPass.getAttachments().begin() + 0u ), depthView );
			attaches.emplace_back( *( renderPass.getAttachments().begin() + 1u ), colourView );
			auto dimensions = colourView.getTexture().getDimensions();
			return renderPass.createFrameBuffer( ashes::Extent2D{ dimensions.width, dimensions.height }
				, std::move( attaches ) );
		}

		ashes::DescriptorSetLayoutPtr doCreateGBufferDescriptorLayout( ashes::Device const & device )
		{
			std::vector< ashes::DescriptorSetLayoutBinding > bindings
			{
				ashes::DescriptorSetLayoutBinding{ 0u, ashes::DescriptorType::eCombinedImageSampler, ashes::ShaderStageFlag::eFragment },
				ashes::DescriptorSetLayoutBinding{ 1u, ashes::DescriptorType::eCombinedImageSampler, ashes::ShaderStageFlag::eFragment },
				ashes::DescriptorSetLayoutBinding{ 2u, ashes::DescriptorType::eCombinedImageSampler, ashes::ShaderStageFlag::eFragment },
				ashes::DescriptorSetLayoutBinding{ 3u, ashes::DescriptorType::eCombinedImageSampler, ashes::ShaderStageFlag::eFragment },
				ashes::DescriptorSetLayoutBinding{ 4u, ashes::DescriptorType::eCombinedImageSampler, ashes::ShaderStageFlag::eFragment },
			};
			return device.createDescriptorSetLayout( std::move( bindings ) );
		}

		ashes::DescriptorSetLayoutPtr doCreateUboDescriptorLayout( ashes::Device const & device )
		{
			std::vector< ashes::DescriptorSetLayoutBinding > bindings
			{
				ashes::DescriptorSetLayoutBinding{ 0u, ashes::DescriptorType::eUniformBuffer, ashes::ShaderStageFlag::eFragment },
				ashes::DescriptorSetLayoutBinding{ 1u, ashes::DescriptorType::eUniformBuffer, ashes::ShaderStageFlag::eFragment },
			};
			return device.createDescriptorSetLayout( std::move( bindings ) );
		}

		ashes::DescriptorSetPtr doCreateUboDescriptorSet( ashes::DescriptorSetPool const & pool
			, ashes::UniformBuffer< common::LightsData > const & lightsUbo
			, ashes::UniformBuffer< common::SceneData > const & sceneUbo )
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

		ashes::VertexBufferPtr< common::TexturedVertexData > doCreateVertexBuffer( ashes::Device const & device
			, ashes::StagingBuffer & stagingBuffer
			, ashes::CommandBuffer const & commandBuffer )
		{
			std::vector< common::TexturedVertexData > vertexData
			{
				{ { -1.0, -1.0, 0.0, 1.0 }, { 0.0, 0.0 } },
				{ { -1.0, +1.0, 0.0, 1.0 }, { 0.0, 1.0 } },
				{ { +1.0, -1.0, 0.0, 1.0 }, { 1.0, 0.0 } },
				{ { +1.0, +1.0, 0.0, 1.0 }, { 1.0, 1.0 } },
			};
			auto result = ashes::makeVertexBuffer< common::TexturedVertexData >( device
				, uint32_t( vertexData.size() )
				, ashes::BufferTarget::eTransferDst
				, ashes::MemoryPropertyFlag::eDeviceLocal );
			stagingBuffer.uploadVertexData( commandBuffer
				, vertexData
				, *result );
			return result;
		}

		ashes::VertexLayoutPtr doCreateVertexLayout( ashes::Device const & device )
		{
			auto result = ashes::makeLayout< common::TexturedVertexData >( 0 );
			result->createAttribute( 0u
				, ashes::Format::eR32G32B32A32_SFLOAT
				, uint32_t( offsetof( common::TexturedVertexData, position ) ) );
			result->createAttribute( 1u
				, ashes::Format::eR32G32_SFLOAT
				, uint32_t( offsetof( common::TexturedVertexData, uv ) ) );
			return result;
		}
	}

	LightingPass::LightingPass( ashes::Device const & device
		, ashes::UniformBuffer< common::LightsData > const & lightsUbo
		, ashes::StagingBuffer & stagingBuffer
		, ashes::TextureViewCRefArray const & views )
		: m_device{ device }
		, m_lightsUbo{ lightsUbo }
		, m_updateCommandBuffer{ m_device.getGraphicsCommandPool().createCommandBuffer() }
		, m_commandBuffer{ m_device.getGraphicsCommandPool().createCommandBuffer() }
		, m_sceneUbo{ ashes::makeUniformBuffer< common::SceneData >( device, 1u, ashes::BufferTarget::eTransferDst, ashes::MemoryPropertyFlag::eDeviceLocal ) }
		, m_gbufferDescriptorLayout{ doCreateGBufferDescriptorLayout( m_device ) }
		, m_gbufferDescriptorPool{ m_gbufferDescriptorLayout->createPool( 1u, false ) }
		, m_uboDescriptorLayout{ doCreateUboDescriptorLayout( m_device ) }
		, m_uboDescriptorPool{ m_uboDescriptorLayout->createPool( 1u ) }
		, m_uboDescriptorSet{ doCreateUboDescriptorSet( *m_uboDescriptorPool, m_lightsUbo, *m_sceneUbo ) }
		, m_renderPass{ doCreateRenderPass( m_device, views[0].get(), views[1].get() ) }
		, m_sampler{ m_device.createSampler( ashes::WrapMode::eClampToEdge
			, ashes::WrapMode::eClampToEdge
			, ashes::WrapMode::eClampToEdge
			, ashes::Filter::eNearest
			, ashes::Filter::eNearest ) }
		, m_vertexBuffer{ doCreateVertexBuffer( m_device, stagingBuffer, *m_updateCommandBuffer ) }
		, m_vertexLayout{ doCreateVertexLayout( m_device ) }
		, m_pipelineLayout{ m_device.createPipelineLayout( { *m_gbufferDescriptorLayout, *m_uboDescriptorLayout } ) }
		, m_pipeline{ m_pipelineLayout->createPipeline( 
			{
				doCreateProgram( m_device ),
				*m_renderPass,
				ashes::VertexInputState::create( *m_vertexLayout ),
				{ ashes::PrimitiveTopology::eTriangleStrip },
				ashes::RasterisationState{},
				ashes::MultisampleState{},
				ashes::ColourBlendState::createDefault(),
				{ ashes::DynamicState::eViewport, ashes::DynamicState::eScissor },
				ashes::DepthStencilState{ 0u, false, false, ashes::CompareOp::eLess }
			} )
		}
		, m_queryPool{ m_device.createQueryPool( ashes::QueryType::eTimestamp, 2u, 0u ) }
	{
	}

	void LightingPass::update( common::SceneData const & sceneData
		, ashes::StagingBuffer & stagingBuffer
		, ashes::TextureViewCRefArray const & views
		, GeometryPassResult const & geometryBuffers )
	{
		m_geometryBuffers = &geometryBuffers;
		m_depthView = &views[0].get();
		m_colourView = &views[1].get();

		m_sceneUbo->getData( 0u ).mtxProjection = utils::inverse( sceneData.mtxProjection );
		stagingBuffer.uploadUniformData( *m_updateCommandBuffer
			, m_sceneUbo->getDatas()
			, *m_sceneUbo
			, ashes::PipelineStageFlag::eFragmentShader );

		auto dimensions = m_depthView->getTexture().getDimensions();
		auto size = ashes::Extent2D{ dimensions.width, dimensions.height };
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
		static ashes::DepthStencilClearValue const depth{ 1.0, 0 };
		static ashes::ClearColorValue const colour{ 1.0f, 0.8f, 0.4f, 0.0f };

		commandBuffer.begin( ashes::CommandBufferUsageFlag::eSimultaneousUse );
		commandBuffer.resetQueryPool( *m_queryPool, 0u, 2u );
		commandBuffer.writeTimestamp( ashes::PipelineStageFlag::eTopOfPipe
			, *m_queryPool
			, 0u );

		for ( auto & texture : gbuffer )
		{
			commandBuffer.memoryBarrier( ashes::PipelineStageFlag::eColourAttachmentOutput
				, ashes::PipelineStageFlag::eFragmentShader
				, texture.view->makeShaderInputResource( ashes::ImageLayout::eColourAttachmentOptimal
					, ashes::AccessFlag::eColourAttachmentWrite ) );
		}

		commandBuffer.beginRenderPass( *m_renderPass
			, *m_frameBuffer
			, { depth, colour }
			, ashes::SubpassContents::eInline );
		commandBuffer.bindPipeline( *m_pipeline );
		commandBuffer.setViewport( { size.width
			, size.height
			, 0
			, 0 } );
		commandBuffer.setScissor( { 0
			, 0
			, size.width
			, size.height } );
		commandBuffer.bindVertexBuffer( 0u, m_vertexBuffer->getBuffer(), 0u );
		commandBuffer.bindDescriptorSet( *m_gbufferDescriptorSet
			, *m_pipelineLayout );
		commandBuffer.bindDescriptorSet( *m_uboDescriptorSet
			, *m_pipelineLayout );
		commandBuffer.draw( 4u );
		commandBuffer.endRenderPass();
		commandBuffer.writeTimestamp( ashes::PipelineStageFlag::eTopOfPipe
			, *m_queryPool
			, 1u );
		commandBuffer.end();
	}

	void LightingPass::draw( std::chrono::nanoseconds & gpu )const
	{
		m_device.getGraphicsQueue().submit( *m_commandBuffer, nullptr );

		ashes::UInt64Array values{ 0u, 0u };
		m_queryPool->getResults( 0u
			, 2u
			, 0u
			, ashes::QueryResultFlag::eWait
			, values );
		gpu += std::chrono::nanoseconds{ uint64_t( ( values[1] - values[0] ) / float( m_device.getTimestampPeriod() ) ) };
	}
}
