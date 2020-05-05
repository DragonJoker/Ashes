#include "EquirectangularToCube.hpp"

#include <ashespp/Buffer/StagingBuffer.hpp>
#include <ashespp/Buffer/UniformBuffer.hpp>
#include <ashespp/Buffer/VertexBuffer.hpp>
#include <ashespp/Sync/Queue.hpp>
#include <ashespp/Core/Device.hpp>
#include <ashespp/Descriptor/DescriptorSet.hpp>
#include <ashespp/Descriptor/DescriptorSetLayout.hpp>
#include <ashespp/Descriptor/DescriptorSetPool.hpp>
#include <ashespp/Image/Image.hpp>
#include <ashespp/Image/ImageView.hpp>
#include <ashespp/RenderPass/FrameBuffer.hpp>
#include <ashespp/RenderPass/RenderPassCreateInfo.hpp>
#include <ashespp/RenderPass/RenderPass.hpp>
#include <util/GlslToSpv.hpp>

#include <util/Transform.hpp>

#include <FileUtils.hpp>

namespace vkapp
{
	using utils::Vec3;
	using utils::Vec4;
	using utils::Mat4;

	namespace
	{
		ashes::ImagePtr doCreateTexture( utils::Device const & device
			, ashes::Queue const & queue
			, ashes::CommandPool const & commandPool
			, common::ImageData const & image )
		{
			auto result = device.createImage(
				{
					0u,
					VK_IMAGE_TYPE_2D,
					image.format,
					VkExtent3D{ image.size.width, image.size.height, 1u },
					1u,
					1u,
					VK_SAMPLE_COUNT_1_BIT,
					VK_IMAGE_TILING_OPTIMAL,
					VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT
				}
				, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
			auto view = result->createView( VK_IMAGE_VIEW_TYPE_2D
				, image.format );
			
			auto staging = ashes::StagingBuffer{ device, 0u, ashes::getSize( image.size, image.format ) };
			staging.uploadTextureData( queue
				, commandPool
				, image.format
				, image.data
				, view );
			return result;
		}

		ashes::SamplerPtr doCreateSampler( utils::Device const & device )
		{
			return device->createSampler( VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
				, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
				, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
				, VK_FILTER_LINEAR
				, VK_FILTER_LINEAR );
		}

		ashes::UniformBufferPtr doCreateMatrixUbo( utils::Device const & device
			, ashes::Queue const & queue
			, ashes::CommandPool const & commandPool
			, ashes::StagingBuffer & stagingBuffer
			, std::array< utils::Mat4, 6u > & matrixData )
		{
			static Mat4 const projection = utils::Mat4{ device->perspective( float( utils::toRadians( 90.0_degrees ) ), 1.0f, 0.1f, 10.0f ) };
			matrixData = [&device]()
			{
				std::array< Mat4, 6u > result
				{
					utils::lookAt( Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ +1.0f, +0.0f, +0.0f }, Vec3{ 0.0f, -1.0f, +0.0f } ),
					utils::lookAt( Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ -1.0f, +0.0f, +0.0f }, Vec3{ 0.0f, -1.0f, +0.0f } ),
					utils::lookAt( Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ +0.0f, +1.0f, +0.0f }, Vec3{ 0.0f, +0.0f, +1.0f } ),
					utils::lookAt( Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ +0.0f, -1.0f, +0.0f }, Vec3{ 0.0f, +0.0f, -1.0f } ),
					utils::lookAt( Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ +0.0f, +0.0f, +1.0f }, Vec3{ 0.0f, -1.0f, +0.0f } ),
					utils::lookAt( Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ +0.0f, +0.0f, -1.0f }, Vec3{ 0.0f, -1.0f, +0.0f } )
				};

				//if ( device->getInstance().getName().find( "gl" ) != std::string::npos )
				//{
				//	std::swap( result[2], result[3] );
				//}

				return result;
			}();

			auto result = utils::makeUniformBuffer( device
				, 6u
				, uint32_t( sizeof( utils::Mat4 ) )
				, VK_BUFFER_USAGE_TRANSFER_DST_BIT
				, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );

			matrixData[0] = projection * matrixData[0];
			matrixData[1] = projection * matrixData[1];
			matrixData[2] = projection * matrixData[2];
			matrixData[3] = projection * matrixData[3];
			matrixData[4] = projection * matrixData[4];
			matrixData[5] = projection * matrixData[5];

			stagingBuffer.uploadUniformData( queue
				, commandPool
				, matrixData
				, *result
				, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT );
			return result;
		}

		ashes::PipelineShaderStageCreateInfoArray doCreateProgram( utils::Device const & device )
		{
			std::string shadersFolder = ashes::getPath( ashes::getExecutableDirectory() ) / "share" / AppName / "Shaders";

			if ( !wxFileExists( shadersFolder / "equirectangular.vert" )
				|| !wxFileExists( shadersFolder / "equirectangular.frag" ) )
			{
				throw std::runtime_error{ "Shader files are missing" };
			}

			ashes::PipelineShaderStageCreateInfoArray shaderStages;
			shaderStages.push_back( ashes::PipelineShaderStageCreateInfo
				{
					0u,
					VK_SHADER_STAGE_VERTEX_BIT,
					device->createShaderModule( common::parseShaderFile( device
						, VK_SHADER_STAGE_VERTEX_BIT
						, shadersFolder / "equirectangular.vert" ) ),
					"main",
					ashes::nullopt,
				} );
			shaderStages.push_back( ashes::PipelineShaderStageCreateInfo
				{
					0u,
					VK_SHADER_STAGE_FRAGMENT_BIT,
					device->createShaderModule( common::parseShaderFile( device
						, VK_SHADER_STAGE_FRAGMENT_BIT
						, shadersFolder / "equirectangular.frag" ) ),
					"main",
					ashes::nullopt,
				} );

			return shaderStages;
		}

		ashes::VertexBufferPtr< VertexData > doCreateVertexBuffer( utils::Device const & device
			, ashes::Queue const & queue
			, ashes::CommandPool const & commandPool
			, ashes::StagingBuffer & stagingBuffer )
		{
			std::vector< VertexData > vertexData
			{
				{
					{ Vec4{ -1, +1, -1, 1 } }, { Vec4{ +1, -1, -1, 1 } }, { Vec4{ -1, -1, -1, 1 } }, { Vec4{ +1, -1, -1, 1 } }, { Vec4{ -1, +1, -1, 1 } }, { Vec4{ +1, +1, -1, 1 } },// Back
					{ Vec4{ -1, -1, +1, 1 } }, { Vec4{ -1, +1, -1, 1 } }, { Vec4{ -1, -1, -1, 1 } }, { Vec4{ -1, +1, -1, 1 } }, { Vec4{ -1, -1, +1, 1 } }, { Vec4{ -1, +1, +1, 1 } },// Left
					{ Vec4{ +1, -1, -1, 1 } }, { Vec4{ +1, +1, +1, 1 } }, { Vec4{ +1, -1, +1, 1 } }, { Vec4{ +1, +1, +1, 1 } }, { Vec4{ +1, -1, -1, 1 } }, { Vec4{ +1, +1, -1, 1 } },// Right
					{ Vec4{ -1, -1, +1, 1 } }, { Vec4{ +1, +1, +1, 1 } }, { Vec4{ -1, +1, +1, 1 } }, { Vec4{ +1, +1, +1, 1 } }, { Vec4{ -1, -1, +1, 1 } }, { Vec4{ +1, -1, +1, 1 } },// Front
					{ Vec4{ -1, +1, -1, 1 } }, { Vec4{ +1, +1, +1, 1 } }, { Vec4{ +1, +1, -1, 1 } }, { Vec4{ +1, +1, +1, 1 } }, { Vec4{ -1, +1, -1, 1 } }, { Vec4{ -1, +1, +1, 1 } },// Top
					{ Vec4{ -1, -1, -1, 1 } }, { Vec4{ +1, -1, -1, 1 } }, { Vec4{ -1, -1, +1, 1 } }, { Vec4{ +1, -1, -1, 1 } }, { Vec4{ +1, -1, +1, 1 } }, { Vec4{ -1, -1, +1, 1 } },// Bottom
				}
			};
			auto result = utils::makeVertexBuffer< VertexData >( device
				, 36u
				, VK_BUFFER_USAGE_TRANSFER_DST_BIT
				, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );

			stagingBuffer.uploadVertexData( queue
				, commandPool
				, vertexData
				, *result );

			return result;
		}

		ashes::PipelineVertexInputStateCreateInfo doCreateVertexLayout( utils::Device const & device )
		{
			ashes::PipelineVertexInputStateCreateInfo result
			{
				0u,
				{
					{ 0u, sizeof( VertexData ), VK_VERTEX_INPUT_RATE_VERTEX },
				},
				{
					{ 0u, 0u, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof( VertexData, position ) },
				},
			};

			return result;
		}

		ashes::DescriptorSetLayoutPtr doCreateDescriptorSetLayout( utils::Device const & device )
		{
			ashes::VkDescriptorSetLayoutBindingArray bindings
			{
				{ 0u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1u, VK_SHADER_STAGE_VERTEX_BIT, nullptr },
				{ 1u, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1u, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr },
			};
			return device->createDescriptorSetLayout( std::move( bindings ) );
		}

		ashes::RenderPassPtr doCreateRenderPass( utils::Device const & device
			, VkFormat format )
		{
			ashes::VkAttachmentDescriptionArray attaches
			{
				{
					0u,
					VK_FORMAT_R8G8B8A8_UNORM,
					VK_SAMPLE_COUNT_1_BIT,
					VK_ATTACHMENT_LOAD_OP_CLEAR,
					VK_ATTACHMENT_STORE_OP_STORE,
					VK_ATTACHMENT_LOAD_OP_DONT_CARE,
					VK_ATTACHMENT_STORE_OP_DONT_CARE,
					VK_IMAGE_LAYOUT_UNDEFINED,
					VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
				},
			};
			ashes::SubpassDescriptionArray subpasses;
			subpasses.emplace_back( ashes::SubpassDescription
				{
					0u,
					VK_PIPELINE_BIND_POINT_GRAPHICS,
					{},
					{ { 0u, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } },
					{},
					ashes::nullopt,
					{},
				} );
			ashes::VkSubpassDependencyArray dependencies
			{
				{
					VK_SUBPASS_EXTERNAL,
					0u,
					VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					0u,
					VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
					VK_DEPENDENCY_BY_REGION_BIT,
				},
				{
					0u,
					VK_SUBPASS_EXTERNAL,
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
					VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
					VK_ACCESS_SHADER_READ_BIT,
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
			return device->createRenderPass( std::move( createInfo ) );
		}
	}

	EquirectangularToCube::EquirectangularToCube( std::string const & filePath
		, utils::Device const & device
		, ashes::Queue const & queue
		, ashes::CommandPool const & commandPool
		, ashes::Image & texture )
		: m_device{ device }
		, m_queue{ queue }
		, m_commandBuffer{ commandPool.createCommandBuffer() }
		, m_image{ common::loadImage( filePath ) }
		, m_stagingBuffer{ device, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, uint32_t( m_image.data.size() ) }
		, m_texture{ doCreateTexture( m_device, queue, commandPool, m_image ) }
		, m_view{ m_texture->createView( VK_IMAGE_VIEW_TYPE_2D, m_image.format ) }
		, m_sampler{ doCreateSampler( m_device ) }
		, m_matrixUbo{ doCreateMatrixUbo( m_device, queue, commandPool, m_stagingBuffer, m_matrixData ) }
		, m_vertexBuffer{ doCreateVertexBuffer( m_device, queue, commandPool, m_stagingBuffer ) }
		, m_vertexLayout{ doCreateVertexLayout( m_device ) }
		, m_descriptorLayout{ doCreateDescriptorSetLayout( m_device ) }
		, m_descriptorPool{ m_descriptorLayout->createPool( 6u ) }
		, m_pipelineLayout{ m_device->createPipelineLayout( *m_descriptorLayout ) }
		, m_renderPass{ doCreateRenderPass( m_device, texture.getFormat() ) }
	{
		auto size = VkExtent2D{ texture.getDimensions().width, texture.getDimensions().height };
		uint32_t face = 0u;
		VkViewport viewport{ 0.0f, 0.0f, float( size.width ), float( size.height ), 0.0f, 1.0f };
		VkRect2D scissor{ { 0, 0 }, size };

		for ( auto & facePipeline : m_faces )
		{
			ashes::ImageViewCRefArray attaches;
			facePipeline.view = texture.createView( VK_IMAGE_VIEW_TYPE_2D
				, texture.getFormat()
				, 0u
				, 1u
				, face
				, 1u );
			attaches.emplace_back( facePipeline.view );
			facePipeline.frameBuffer = m_renderPass->createFrameBuffer( size
				, std::move( attaches ) );

			facePipeline.pipeline = m_device->createPipeline( ashes::GraphicsPipelineCreateInfo
				{
					0u,
					doCreateProgram( m_device ),
					doCreateVertexLayout( m_device ),
					ashes::PipelineInputAssemblyStateCreateInfo{ 0u, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST },
					ashes::nullopt,
					ashes::PipelineViewportStateCreateInfo{ 0u, 1u, { viewport }, 1u, { scissor } },
					ashes::PipelineRasterizationStateCreateInfo{ 0u, VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE },
					ashes::PipelineMultisampleStateCreateInfo{},
					ashes::nullopt,
					ashes::PipelineColorBlendStateCreateInfo{},
					ashes::PipelineDynamicStateCreateInfo{},
					*m_pipelineLayout,
					*m_renderPass,
				} );

			facePipeline.descriptorSet = m_descriptorPool->createDescriptorSet();
			facePipeline.descriptorSet->createBinding( m_descriptorLayout->getBinding( 0u )
				, *m_matrixUbo
				, face
				, 1u );
			facePipeline.descriptorSet->createBinding( m_descriptorLayout->getBinding( 1u )
				, m_view
				, *m_sampler );
			facePipeline.descriptorSet->update();
			++face;
		}
	}

	void EquirectangularToCube::render( ashes::CommandBuffer & commandBuffer )
	{
		for ( auto & facePipeline : m_faces )
		{
			commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
				, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
				, ( *facePipeline.frameBuffer->begin() ).get().makeColourAttachment( VK_IMAGE_LAYOUT_UNDEFINED ) );
			commandBuffer.beginRenderPass( *m_renderPass
				, *facePipeline.frameBuffer
				, { ashes::makeClearValue( VkClearColorValue{ 0, 0, 0, 0 } ) }
				, VK_SUBPASS_CONTENTS_INLINE );
			commandBuffer.bindPipeline( *facePipeline.pipeline );
			commandBuffer.bindDescriptorSet( *facePipeline.descriptorSet
				, *m_pipelineLayout );
			commandBuffer.bindVertexBuffer( 0u, m_vertexBuffer->getBuffer(), 0u );
			commandBuffer.draw( m_vertexBuffer->getCount() );
			commandBuffer.endRenderPass();
		}

	}

	void EquirectangularToCube::render()
	{
		m_commandBuffer->begin( VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT );
		render( *m_commandBuffer );
		m_commandBuffer->end();
		m_queue.submit( *m_commandBuffer, nullptr );
		m_queue.waitIdle();
	}
}
