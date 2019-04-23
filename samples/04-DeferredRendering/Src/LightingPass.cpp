#include "LightingPass.hpp"

#include <AshesPP/Buffer/Buffer.hpp>
#include <AshesPP/Buffer/StagingBuffer.hpp>
#include <AshesPP/Buffer/VertexBuffer.hpp>
#include <AshesPP/Command/CommandBuffer.hpp>
#include <AshesPP/Command/CommandPool.hpp>
#include <AshesPP/Core/Device.hpp>
#include <AshesPP/Descriptor/DescriptorSet.hpp>
#include <AshesPP/Descriptor/DescriptorSetLayout.hpp>
#include <AshesPP/Descriptor/DescriptorSetLayoutBinding.hpp>
#include <AshesPP/Descriptor/DescriptorSetPool.hpp>
#include <AshesPP/Image/Sampler.hpp>
#include <AshesPP/Image/Image.hpp>
#include <AshesPP/Image/ImageView.hpp>
#include <AshesPP/Miscellaneous/QueryPool.hpp>
#include <AshesPP/Pipeline/DepthStencilState.hpp>
#include <AshesPP/Pipeline/InputAssemblyState.hpp>
#include <AshesPP/Pipeline/MultisampleState.hpp>
#include <AshesPP/Pipeline/GraphicsPipeline.hpp>
#include <AshesPP/Pipeline/PipelineLayout.hpp>
#include <AshesPP/Pipeline/Scissor.hpp>
#include <AshesPP/Pipeline/VertexLayout.hpp>
#include <AshesPP/Pipeline/Viewport.hpp>
#include <AshesPP/RenderPass/FrameBuffer.hpp>
#include <AshesPP/RenderPass/RenderPass.hpp>
#include <AshesPP/RenderPass/RenderSubpass.hpp>
#include <AshesPP/RenderPass/RenderSubpassState.hpp>
#include <AshesPP/RenderPass/FrameBufferAttachment.hpp>
#include <AshesPP/Sync/ImageMemoryBarrier.hpp>

#include <Utils/GlslToSpv.hpp>

#include <FileUtils.hpp>

namespace vkapp
{
	namespace
	{
		ashes::PipelineShaderStageCreateInfoArray doCreateProgram( ashes::Device const & device )
		{
			std::string shadersFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / AppName / "Shaders";

			if ( !wxFileExists( shadersFolder / "opaque_lp.vert" )
				|| !wxFileExists( shadersFolder / "opaque_lp.frag" ) )
			{
				throw std::runtime_error{ "Shader files are missing" };
			}

			ashes::PipelineShaderStageCreateInfoArray shaderStages;
			shaderStages.push_back( { device.createShaderModule( VK_SHADER_STAGE_VERTEX_BIT ) } );
			shaderStages.push_back( { device.createShaderModule( VK_SHADER_STAGE_FRAGMENT_BIT ) } );
			shaderStages[0].module->loadShader( common::dumpShaderFile( device, VK_SHADER_STAGE_VERTEX_BIT, shadersFolder / "opaque_lp.vert" ) );
			shaderStages[1].module->loadShader( common::dumpShaderFile( device, VK_SHADER_STAGE_FRAGMENT_BIT, shadersFolder / "opaque_lp.frag" ) );
			return shaderStages;
		}

		std::vector< VkFormat > doGetFormats( ashes::ImageView const & depthView
			, ashes::ImageView const & colourView )
		{
			return
			{
				depthView->format,
				colourView->format,
			};
		}

		ashes::VkAttachmentDescriptionArray doGetAttaches( ashes::ImageView const & depthView
			, ashes::ImageView const & colourView )
		{
			return ashes::VkAttachmentDescriptionArray
			{
				{
					depthView->format,
					VK_SAMPLE_COUNT_1_BIT,
					VkAttachmentLoadOp::eLoad,
					VK_ATTACHMENT_STORE_OP_STORE,
					VkAttachmentLoadOp::eLoad,
					VK_ATTACHMENT_STORE_OP_STORE,
					VK_IMAGE_LAYOUT_UNDEFINED,
					VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
				},
				{
					colourView->format,
					VK_SAMPLE_COUNT_1_BIT,
					VK_ATTACHMENT_LOAD_OP_CLEAR,
					VK_ATTACHMENT_STORE_OP_STORE,
					VK_ATTACHMENT_LOAD_OP_DONT_CARE,
					VK_ATTACHMENT_STORE_OP_DONT_CARE,
					VK_IMAGE_LAYOUT_UNDEFINED,
					VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				}
			};
		}

		ashes::RenderPassPtr doCreateRenderPass( ashes::Device const & device
			, ashes::ImageViewPtr depthView
			, ashes::ImageViewPtr colourView )
		{
			ashes::VkAttachmentReferenceArray subAttaches
			{
				VkAttachmentReference{ 1u, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL },
			};
			ashes::RenderSubpassPtrArray subpasses;
			subpasses.emplace_back( std::make_unique< ashes::RenderSubpass >( VkPipelineBindPoint::eGraphics
				, ashes::RenderSubpassState{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT }
				, subAttaches
				, VkAttachmentReference{ 0u, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL } ) );
			return device.createRenderPass( doGetAttaches( *depthView, *colourView )
				, std::move( subpasses )
				, ashes::RenderSubpassState{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
					, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT }
				, ashes::RenderSubpassState{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
					, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT } );
		}

		ashes::FrameBufferPtr doCreateFrameBuffer( ashes::RenderPass const & renderPass
			, ashes::ImageViewPtr depthView
			, ashes::ImageViewPtr colourView )
		{
			auto formats = doGetFormats( *depthView, *colourView );
			ashes::ImageViewArray attaches;
			attaches.emplace_back( *( renderPass.getAttachments().begin() + 0u )
				, depthView );
			attaches.emplace_back( *( renderPass.getAttachments().begin() + 1u )
				, colourView );
			auto dimensions = colourView->getImage().getDimensions();
			return renderPass.createFrameBuffer( VkExtent2D{ dimensions.width, dimensions.height }
				, std::move( attaches ) );
		}

		ashes::DescriptorSetLayoutPtr doCreateGBufferDescriptorLayout( ashes::Device const & device )
		{
			VkDescriptorSetLayoutBindingArray bindings
			{
				VkDescriptorSetLayoutBinding{ 0u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT },
				VkDescriptorSetLayoutBinding{ 1u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT },
				VkDescriptorSetLayoutBinding{ 2u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT },
				VkDescriptorSetLayoutBinding{ 3u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT },
				VkDescriptorSetLayoutBinding{ 4u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT },
			};
			return device.createDescriptorSetLayout( std::move( bindings ) );
		}

		ashes::DescriptorSetLayoutPtr doCreateUboDescriptorLayout( ashes::Device const & device )
		{
			VkDescriptorSetLayoutBindingArray bindings
			{
				VkDescriptorSetLayoutBinding{ 0u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT },
				VkDescriptorSetLayoutBinding{ 1u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT },
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

		ashes::VertexBufferPtr< common::TexturedVertexData > doCreateVertexBuffer( utils::Device const & device
			, ashes::StagingBuffer & stagingBuffer
			, ashes::CommandPool const & commandPool
			, ashes::Queue const & transferQueue )
		{
			std::vector< common::TexturedVertexData > vertexData
			{
				{ { -1.0, -1.0, 0.0, 1.0 }, { 0.0, 0.0 } },
				{ { -1.0, +1.0, 0.0, 1.0 }, { 0.0, 1.0 } },
				{ { +1.0, -1.0, 0.0, 1.0 }, { 1.0, 0.0 } },
				{ { +1.0, +1.0, 0.0, 1.0 }, { 1.0, 1.0 } },
			};
			auto result = utils::makeVertexBuffer< common::TexturedVertexData >( device
				, uint32_t( vertexData.size() )
				, VK_BUFFER_USAGE_TRANSFER_DST_BIT
				, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
			stagingBuffer.uploadVertexData( transferQueue
				, commandPool
				, vertexData
				, *result );
			return result;
		}

		ashes::VertexLayoutPtr doCreateVertexLayout( ashes::Device const & device )
		{
			auto result = ashes::makeLayout< common::TexturedVertexData >( 0 );
			result->createAttribute( 0u
				, VK_FORMAT_R32G32B32A32_SFLOAT
				, uint32_t( offsetof( common::TexturedVertexData, position ) ) );
			result->createAttribute( 1u
				, VK_FORMAT_R32G32_SFLOAT
				, uint32_t( offsetof( common::TexturedVertexData, uv ) ) );
			return result;
		}
	}

	LightingPass::LightingPass( utils::Device const & device
		, ashes::CommandPool const & commandPool
		, ashes::Queue const & transferQueue
		, ashes::UniformBuffer< common::LightsData > const & lightsUbo
		, ashes::StagingBuffer & stagingBuffer
		, ashes::ImageViewArray views )
		: m_device{ device }
		, m_commandPool{ commandPool }
		, m_transferQueue{ transferQueue }
		, m_lightsUbo{ lightsUbo }
		, m_commandBuffer{ commandPool.createCommandBuffer() }
		, m_sceneUbo{ utils::makeUniformBuffer< common::SceneData >( device, 1u, VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT ) }
		, m_gbufferDescriptorLayout{ doCreateGBufferDescriptorLayout( m_device.getDevice() ) }
		, m_gbufferDescriptorPool{ m_gbufferDescriptorLayout->createPool( 1u, false ) }
		, m_uboDescriptorLayout{ doCreateUboDescriptorLayout( m_device.getDevice() ) }
		, m_uboDescriptorPool{ m_uboDescriptorLayout->createPool( 1u ) }
		, m_uboDescriptorSet{ doCreateUboDescriptorSet( *m_uboDescriptorPool, m_lightsUbo, *m_sceneUbo ) }
		, m_renderPass{ doCreateRenderPass( m_device.getDevice(), views[0], views[1] ) }
		, m_sampler{ m_device.getDevice().createSampler( VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_FILTER_NEAREST
			, VK_FILTER_NEAREST ) }
		, m_vertexBuffer{ doCreateVertexBuffer( m_device, stagingBuffer, commandPool, transferQueue ) }
		, m_vertexLayout{ doCreateVertexLayout( m_device.getDevice() ) }
		, m_pipelineLayout{ m_device.getDevice().createPipelineLayout( { *m_gbufferDescriptorLayout, *m_uboDescriptorLayout } ) }
		, m_pipeline{ m_pipelineLayout->createPipeline( 
			{
				doCreateProgram( m_device.getDevice() ),
				*m_renderPass,
				ashes::VertexInputState::create( *m_vertexLayout ),
				{ VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP },
				VkPipelineRasterizationStateCreateInfo{},
				VkPipelineMultisampleStateCreateInfo{},
				VkPipelineColorBlendStateCreateInfo::createDefault(),
				{ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR },
				VkPipelineDepthStencilStateCreateInfo{ 0u, false, false, VK_COMPARE_OP_LESS }
			} )
		}
		, m_queryPool{ m_device.getDevice().createQueryPool( VK_QUERY_TYPE_TIMESTAMP, 2u, 0u ) }
	{
	}

	void LightingPass::update( common::SceneData const & sceneData
		, ashes::StagingBuffer & stagingBuffer
		, ashes::ImageViewArray views
		, GeometryPassResult const & geometryBuffers )
	{
		m_geometryBuffers = &geometryBuffers;
		m_depthView = views[0];
		m_colourView = views[1];

		m_sceneUbo->getData( 0u ).mtxProjection = utils::inverse( sceneData.mtxProjection );
		stagingBuffer.uploadUniformData( m_transferQueue
			, m_commandPool
			, m_sceneUbo->getDatas()
			, *m_sceneUbo
			, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT );

		auto dimensions = m_depthView->getImage().getDimensions();
		auto size = VkExtent2D{ dimensions.width, dimensions.height };
		m_frameBuffer = doCreateFrameBuffer( *m_renderPass, m_depthView, m_colourView );
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
		static VkClearDepthStencilValue const depth{ 1.0, 0 };
		static VkClearColorValue const colour{ 1.0f, 0.8f, 0.4f, 0.0f };

		commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT );
		commandBuffer.resetQueryPool( *m_queryPool, 0u, 2u );
		commandBuffer.writeTimestamp( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
			, *m_queryPool
			, 0u );

		for ( auto & texture : gbuffer )
		{
			commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
				, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
				, texture.view->makeShaderInputResource( VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
					, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT ) );
		}

		commandBuffer.beginRenderPass( *m_renderPass
			, *m_frameBuffer
			, { depth, colour }
			, VK_SUBPASS_CONTENTS_INLINE );
		commandBuffer.bindPipeline( *m_pipeline );
		commandBuffer.setViewport( { 0.0f, 0.0f, float( size.width ), float( size.height ), 0.0f, 1.0f } );
		commandBuffer.setScissor( { { 0, 0 }, { size.width, size.height } } );
		commandBuffer.bindVertexBuffer( 0u, m_vertexBuffer->getBuffer(), 0u );
		commandBuffer.bindDescriptorSet( *m_gbufferDescriptorSet
			, *m_pipelineLayout );
		commandBuffer.bindDescriptorSet( *m_uboDescriptorSet
			, *m_pipelineLayout );
		commandBuffer.draw( 4u );
		commandBuffer.endRenderPass();
		commandBuffer.writeTimestamp( VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
			, *m_queryPool
			, 1u );
		commandBuffer.end();
	}

	void LightingPass::draw( ashes::Queue const & queue
		, std::chrono::nanoseconds & gpu )const
	{
		queue.submit( *m_commandBuffer, nullptr );

		ashes::UInt64Array values{ 0u, 0u };
		m_queryPool->getResults( 0u
			, 2u
			, 0u
			, VK_QUERY_RESULT_WAIT_BIT
			, values );
		gpu += std::chrono::nanoseconds{ uint64_t( ( values[1] - values[0] ) / float( m_device.getDevice().getTimestampPeriod() ) ) };
	}
}
