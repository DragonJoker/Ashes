#include "LightingPass.hpp"

#include <ashespp/Buffer/Buffer.hpp>
#include <ashespp/Buffer/StagingBuffer.hpp>
#include <ashespp/Buffer/VertexBuffer.hpp>
#include <ashespp/Command/CommandBuffer.hpp>
#include <ashespp/Command/CommandPool.hpp>
#include <ashespp/Core/Device.hpp>
#include <ashespp/Descriptor/DescriptorSet.hpp>
#include <ashespp/Descriptor/DescriptorSetLayout.hpp>
#include <ashespp/Descriptor/DescriptorSetPool.hpp>
#include <ashespp/Image/Sampler.hpp>
#include <ashespp/Image/Image.hpp>
#include <ashespp/Image/ImageView.hpp>
#include <ashespp/Miscellaneous/QueryPool.hpp>
#include <ashespp/Pipeline/GraphicsPipeline.hpp>
#include <ashespp/Pipeline/PipelineLayout.hpp>
#include <ashespp/RenderPass/FrameBuffer.hpp>
#include <ashespp/RenderPass/RenderPass.hpp>

#include <util/GlslToSpv.hpp>

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
				throw common::Exception{ "Shader files are missing" };
			}
			
			ashes::PipelineShaderStageCreateInfoArray result;
			result.emplace_back( 0u
				, VK_SHADER_STAGE_VERTEX_BIT
				, device.createShaderModule( common::dumpShaderFile( device
					, VK_SHADER_STAGE_VERTEX_BIT
					, shadersFolder / "opaque_lp.vert" ) )
				, "main"
				, ashes::nullopt );
			result.emplace_back( 0u
				, VK_SHADER_STAGE_FRAGMENT_BIT
				, device.createShaderModule( common::dumpShaderFile( device
					, VK_SHADER_STAGE_FRAGMENT_BIT
					, shadersFolder / "opaque_lp.frag" ) )
				, "main"
				, ashes::nullopt );
			return result;
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
					0u,
					depthView->format,
					VK_SAMPLE_COUNT_1_BIT,
					VK_ATTACHMENT_LOAD_OP_LOAD,
					VK_ATTACHMENT_STORE_OP_STORE,
					VK_ATTACHMENT_LOAD_OP_LOAD,
					VK_ATTACHMENT_STORE_OP_STORE,
					VK_IMAGE_LAYOUT_UNDEFINED,
					VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
				},
				{
					1u,
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
			, ashes::ImageView const & depthView
			, ashes::ImageView const & colourView )
		{
			auto attaches = doGetAttaches( depthView, colourView );
			ashes::SubpassDescriptionArray subpasses;
			subpasses.emplace_back( ashes::SubpassDescription
				{
					0u,
					VK_PIPELINE_BIND_POINT_GRAPHICS,
					{},
					{ { 1u, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } },
					{},
					VkAttachmentReference{ 0u, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL },
					{},
				} );
			ashes::VkSubpassDependencyArray dependencies
			{
				{
					VK_SUBPASS_EXTERNAL,
					0u,
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
					VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
					VK_DEPENDENCY_BY_REGION_BIT,
				},
				{
					0u,
					VK_SUBPASS_EXTERNAL,
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
					VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
					VK_DEPENDENCY_BY_REGION_BIT,
				}
			};
			ashes::RenderPassCreateInfo createInfo
			{
				0u,
				std::move( attaches ),
				std::move( subpasses ),
				std::move( dependencies ),
			};
			return device.createRenderPass( std::move( createInfo ) );
		}

		ashes::FrameBufferPtr doCreateFrameBuffer( ashes::RenderPass const & renderPass
			, ashes::ImageView const & depthView
			, ashes::ImageView const & colourView )
		{
			auto formats = doGetFormats( depthView, colourView );
			ashes::ImageViewCRefArray attaches;
			attaches.emplace_back( depthView );
			attaches.emplace_back( colourView );
			auto dimensions = colourView.image->getDimensions();
			return renderPass.createFrameBuffer( VkExtent2D{ dimensions.width, dimensions.height }
				, std::move( attaches ) );
		}

		ashes::DescriptorSetLayoutPtr doCreateGBufferDescriptorLayout( ashes::Device const & device )
		{
			ashes::VkDescriptorSetLayoutBindingArray bindings
			{
				{ 0u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
				{ 1u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
				{ 2u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
				{ 3u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
				{ 4u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
			};
			return device.createDescriptorSetLayout( std::move( bindings ) );
		}

		ashes::DescriptorSetLayoutPtr doCreateUboDescriptorLayout( ashes::Device const & device )
		{
			ashes::VkDescriptorSetLayoutBindingArray bindings
			{
				{ 0u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
				{ 1u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
			};
			return device.createDescriptorSetLayout( std::move( bindings ) );
		}

		ashes::DescriptorSetPtr doCreateUboDescriptorSet( ashes::DescriptorSetPool const & pool
			, ashes::UniformBuffer const & lightsUbo
			, ashes::UniformBuffer const & sceneUbo )
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
			, ashes::StagingBuffer const & stagingBuffer
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

		ashes::PipelineVertexInputStateCreateInfo doCreateVertexLayout()
		{
			return ashes::PipelineVertexInputStateCreateInfo
			{
				0u,
				{
					{ 0u, sizeof( common::TexturedVertexData ), VK_VERTEX_INPUT_RATE_VERTEX },
				},
				{
					{ 0u, 0u, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof( common::TexturedVertexData, position ) },
					{ 1u, 0u, VK_FORMAT_R32G32_SFLOAT, offsetof( common::TexturedVertexData, uv ) },
				},
			};
		}
	}

	LightingPass::LightingPass( utils::Device const & device
		, ashes::CommandPool const & commandPool
		, ashes::Queue const & transferQueue
		, ashes::UniformBuffer const & lightsUbo
		, ashes::StagingBuffer const & stagingBuffer
		, ashes::ImageViewArray views )
		: m_device{ device }
		, m_commandPool{ commandPool }
		, m_transferQueue{ transferQueue }
		, m_lightsUbo{ lightsUbo }
		, m_commandBuffer{ commandPool.createCommandBuffer() }
		, m_sceneUbo{ utils::makeUniformBuffer( device, 1u, uint32_t( sizeof( common::SceneData ) ), VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT ) }
		, m_uboDescriptorLayout{ doCreateUboDescriptorLayout( m_device.getDevice() ) }
		, m_uboDescriptorPool{ m_uboDescriptorLayout->createPool( 1u ) }
		, m_uboDescriptorSet{ doCreateUboDescriptorSet( *m_uboDescriptorPool, m_lightsUbo, *m_sceneUbo ) }
		, m_gbufferDescriptorLayout{ doCreateGBufferDescriptorLayout( m_device.getDevice() ) }
		, m_gbufferDescriptorPool{ m_gbufferDescriptorLayout->createPool( 1u, false ) }
		, m_renderPass{ doCreateRenderPass( m_device.getDevice(), views[0], views[1] ) }
		, m_sampler{ m_device.getDevice().createSampler( VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
			, VK_FILTER_NEAREST
			, VK_FILTER_NEAREST ) }
		, m_vertexBuffer{ doCreateVertexBuffer( m_device, stagingBuffer, commandPool, transferQueue ) }
		, m_pipelineLayout{ m_device->createPipelineLayout( { *m_gbufferDescriptorLayout, *m_uboDescriptorLayout } ) }
		, m_pipeline{ m_device->createPipeline( ashes::GraphicsPipelineCreateInfo
			{
				0u,
				doCreateProgram( m_device.getDevice() ),
				doCreateVertexLayout(),
				ashes::PipelineInputAssemblyStateCreateInfo{ 0u, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP },
				ashes::nullopt,
				ashes::PipelineViewportStateCreateInfo{},
				ashes::PipelineRasterizationStateCreateInfo{},
				ashes::PipelineMultisampleStateCreateInfo{},
				ashes::PipelineDepthStencilStateCreateInfo{ 0u, false, false, VK_COMPARE_OP_LESS },
				ashes::PipelineColorBlendStateCreateInfo{},
				ashes::PipelineDynamicStateCreateInfo{ 0u, { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR } },
				*m_pipelineLayout,
				*m_renderPass,
			} )
		}
		, m_queryPool{ m_device.getDevice().createQueryPool( VK_QUERY_TYPE_TIMESTAMP, 2u, 0u ) }
	{
	}

	void LightingPass::update( common::SceneData const & sceneData
		, ashes::StagingBuffer const & stagingBuffer
		, ashes::ImageViewArray views
		, GeometryPassResult const & geometryBuffers )
	{
		m_geometryBuffers = &geometryBuffers;
		m_depthView = views[0];
		m_colourView = views[1];

		m_sceneData[0].mtxProjection = utils::inverse( sceneData.mtxProjection );
		stagingBuffer.uploadUniformData( m_transferQueue
			, m_commandPool
			, m_sceneData
			, *m_sceneUbo
			, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT );

		auto dimensions = m_depthView.image->getDimensions();
		auto size = VkExtent2D{ dimensions.width, dimensions.height };
		m_frameBuffer = doCreateFrameBuffer( *m_renderPass, m_depthView, m_colourView );
		m_gbufferDescriptorSet.reset();
		m_gbufferDescriptorSet = m_gbufferDescriptorPool->createDescriptorSet( 0u );
		auto & gbuffer = *m_geometryBuffers;

		for ( size_t i = 0; i < gbuffer.size(); ++i )
		{
			m_gbufferDescriptorSet->createBinding( m_gbufferDescriptorLayout->getBinding( uint32_t( i ) )
				, gbuffer[i].view
				, *m_sampler );
		}

		m_gbufferDescriptorSet->update();
		m_commandBuffer->reset();
		auto const & commandBuffer = *m_commandBuffer;
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
				, texture.view.makeShaderInputResource( VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL ) );
		}

		commandBuffer.beginRenderPass( *m_renderPass
			, *m_frameBuffer
			, { ashes::makeClearValue( depth ), ashes::makeClearValue( colour ) }
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
		gpu += std::chrono::nanoseconds{ uint64_t( float( values[1] - values[0] ) / float( m_device.getDevice().getTimestampPeriod() ) ) };
	}
}
