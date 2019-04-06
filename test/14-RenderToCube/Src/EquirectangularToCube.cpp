#include "EquirectangularToCube.hpp"

#include <Buffer/StagingBuffer.hpp>
#include <Buffer/UniformBuffer.hpp>
#include <Buffer/VertexBuffer.hpp>
#include <AshesPP/Command/Queue.hpp>
#include <AshesPP/Core/Device.hpp>
#include <Descriptor/DescriptorSet.hpp>
#include <Descriptor/DescriptorSetLayout.hpp>
#include <Descriptor/DescriptorSetLayoutBinding.hpp>
#include <Descriptor/DescriptorSetPool.hpp>
#include <Image/Image.hpp>
#include <Image/ImageView.hpp>
#include <Pipeline/DepthStencilState.hpp>
#include <Pipeline/InputAssemblyState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/Scissor.hpp>
#include <Pipeline/VertexLayout.hpp>
#include <Pipeline/Viewport.hpp>
#include <AshesPP/RenderPass/FrameBuffer.hpp>
#include <AshesPP/RenderPass/RenderPassCreateInfo.hpp>
#include <AshesPP/RenderPass/RenderPass.hpp>
#include <AshesPP/RenderPass/RenderSubpass.hpp>
#include <AshesPP/RenderPass/RenderSubpassState.hpp>
#include <GlslToSpv.hpp>
#include <Sync/ImageMemoryBarrier.hpp>

#include <Utils/Transform.hpp>

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
					ashes::ImageType::e2D,
					image.format,
					VkExtent3D{ image.size.width, image.size.height, 1u },
					1u,
					1u,
					VK_SAMPLE_COUNT_1_BIT,
					ashes::ImageTiling::eOptimal,
					VkImageUsageFlagBits::eSampled | VkImageUsageFlagBits::eTransferDst
				}
				, VkMemoryPropertyFlagBits::eDeviceLocal );
			auto view = result->createView( VK_IMAGE_VIEW_TYPE_2D
				, image.format );
			
			auto staging = ashes::StagingBuffer{ device, 0u, ashes::getSize( image.size, image.format ) };
			staging.uploadTextureData( queue
				, commandPool
				, image.format
				, image.data
				, *view );
			return result;
		}

		ashes::SamplerPtr doCreateSampler( utils::Device const & device )
		{
			return device->createSampler( ashes::WrapMode::eClampToEdge
				, ashes::WrapMode::eClampToEdge
				, ashes::WrapMode::eClampToEdge
				, ashes::Filter::eLinear
				, ashes::Filter::eLinear );
		}

		ashes::UniformBufferPtr< Mat4 > doCreateMatrixUbo( utils::Device const & device
			, ashes::Queue const & queue
			, ashes::CommandPool const & commandPool
			, ashes::StagingBuffer & stagingBuffer )
		{
			static Mat4 const projection = utils::Mat4{ device->perspective( float( utils::toRadians( 90.0_degrees ) ), 1.0f, 0.1f, 10.0f ) };
			static std::array< Mat4, 6u > const views = [&device]()
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

				if ( device->getInstance().getName().find( "gl" ) != std::string::npos )
				{
					std::swap( result[2], result[3] );
				}

				return result;
			}();

			auto result = utils::makeUniformBuffer< utils::Mat4 >( device
				, 6u
				, ashes::BufferTarget::eTransferDst
				, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );

			result->getData( 0u ) = projection * views[0];
			result->getData( 1u ) = projection * views[1];
			result->getData( 2u ) = projection * views[2];
			result->getData( 3u ) = projection * views[3];
			result->getData( 4u ) = projection * views[4];
			result->getData( 5u ) = projection * views[5];

			stagingBuffer.uploadUniformData( queue
				, commandPool
				, result->getDatas()
				, *result
				, VkPipelineStageFlagBits::eVertexShader );
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
			shaderStages.push_back( { device->createShaderModule( VK_SHADER_STAGE_VERTEX_BIT ) } );
			shaderStages.push_back( { device->createShaderModule( VK_SHADER_STAGE_FRAGMENT_BIT ) } );
			shaderStages[0].module->loadShader( common::parseShaderFile( device.getDevice()
				, VK_SHADER_STAGE_VERTEX_BIT
				, shadersFolder / "equirectangular.vert" ) );
			shaderStages[1].module->loadShader( common::parseShaderFile( device.getDevice()
				, VK_SHADER_STAGE_FRAGMENT_BIT
				, shadersFolder / "equirectangular.frag" ) );

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
				, ashes::BufferTarget::eTransferDst
				, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );

			stagingBuffer.uploadVertexData( queue
				, commandPool
				, vertexData
				, *result );

			return result;
		}

		ashes::VertexLayoutPtr doCreateVertexLayout( utils::Device const & device )
		{
			auto result = ashes::makeLayout< VertexData >( 0u );
			result->createAttribute( 0u
				, VK_FORMAT_R32G32B32A32_SFLOAT
				, 0u );
			return result;
		}

		ashes::DescriptorSetLayoutPtr doCreateDescriptorSetLayout( utils::Device const & device )
		{
			ashes::VkDescriptorSetLayoutBindingArray bindings
			{
				{ 0u, ashes::DescriptorType::eUniformBuffer, VK_SHADER_STAGE_VERTEX_BIT },
				{ 1u, ashes::DescriptorType::eCombinedImageSampler, VK_SHADER_STAGE_FRAGMENT_BIT },
			};
			return device->createDescriptorSetLayout( std::move( bindings ) );
		}

		ashes::RenderPassPtr doCreateRenderPass( utils::Device const & device
			, VkFormat format )
		{
			ashes::RenderPassCreateInfo renderPass;
			renderPass.flags = 0u;

			renderPass.attachments.resize( 1u );
			renderPass.attachments[0].format = format;
			renderPass.attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			renderPass.attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			renderPass.attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			renderPass.attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			renderPass.attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
			renderPass.attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			renderPass.attachments[0].finalLayout = VkImageLayout::eShaderReadOnlyOptimal;

			renderPass.subpasses.resize( 1u );
			renderPass.subpasses[0].flags = 0u;
			renderPass.subpasses[0].pipelineBindPoint = ashes::PipelineBindPoint::eGraphics;
			renderPass.subpasses[0].colorAttachments.push_back( { 0u, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } );

			renderPass.dependencies.resize( 2u );
			renderPass.dependencies[0].srcSubpass = ashes::ExternalSubpass;
			renderPass.dependencies[0].dstSubpass = 0u;
			renderPass.dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			renderPass.dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			renderPass.dependencies[0].srcAccessMask = 0u;
			renderPass.dependencies[0].dstAccessMask = VkAccessFlagBits::eColourAttachmentWrite;
			renderPass.dependencies[0].dependencyFlags = ashes::DependencyFlag::eByRegion;

			renderPass.dependencies[1].srcSubpass = 0u;
			renderPass.dependencies[1].dstSubpass = ashes::ExternalSubpass;
			renderPass.dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			renderPass.dependencies[1].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			renderPass.dependencies[1].srcAccessMask = VkAccessFlagBits::eColourAttachmentWrite;
			renderPass.dependencies[1].dstAccessMask = VkAccessFlagBits::eShaderRead;
			renderPass.dependencies[1].dependencyFlags = ashes::DependencyFlag::eByRegion;

			return device->createRenderPass( renderPass );
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
		, m_stagingBuffer{ device, ashes::BufferTarget::eTransferSrc, uint32_t( m_image.data.size() ) }
		, m_texture{ doCreateTexture( m_device, queue, commandPool, m_image ) }
		, m_view{ m_texture->createView( VK_IMAGE_VIEW_TYPE_2D, m_image.format ) }
		, m_sampler{ doCreateSampler( m_device ) }
		, m_matrixUbo{ doCreateMatrixUbo( m_device, queue, commandPool, m_stagingBuffer ) }
		, m_vertexBuffer{ doCreateVertexBuffer( m_device, queue, commandPool, m_stagingBuffer ) }
		, m_vertexLayout{ doCreateVertexLayout( m_device ) }
		, m_descriptorLayout{ doCreateDescriptorSetLayout( m_device ) }
		, m_descriptorPool{ m_descriptorLayout->createPool( 6u ) }
		, m_pipelineLayout{ m_device->createPipelineLayout( *m_descriptorLayout ) }
		, m_renderPass{ doCreateRenderPass( m_device, texture.getFormat() ) }
	{
		auto size = VkExtent2D{ texture.getDimensions().width, texture.getDimensions().height };
		uint32_t face = 0u;

		for ( auto & facePipeline : m_faces )
		{
			ashes::ImageViewPtrArray attaches;
			attaches.emplace_back( *m_renderPass->getAttachments().begin()
				, texture.createView( VK_IMAGE_VIEW_TYPE_2D, texture.getFormat(), 0u, 1u, face, 1u ) );
			facePipeline.frameBuffer = m_renderPass->createFrameBuffer( size
				, std::move( attaches ) );

			facePipeline.pipeline = m_pipelineLayout->createPipeline( ashes::GraphicsPipelineCreateInfo
			{
				doCreateProgram( m_device ),
				*m_renderPass,
				ashes::VertexInputState::create( *m_vertexLayout ),
				ashes::InputAssemblyState{ VkPrimitiveTopology::eTriangleList },
				ashes::RasterisationState{ 0u, false, false, ashes::PolygonMode::eFill, ashes::CullModeFlag::eNone },
				ashes::MultisampleState{},
				ashes::ColourBlendState::createDefault(),
				{},
				ashes::DepthStencilState{ 0u, false, false },
				ashes::TessellationState{},
				VkViewport{ size.width, size.height, 0, 0 },
				VkRect2D{ 0, 0, size.width, size.height }
			} );

			facePipeline.descriptorSet = m_descriptorPool->createDescriptorSet();
			facePipeline.descriptorSet->createBinding( m_descriptorLayout->getBinding( 0u )
				, *m_matrixUbo
				, face
				, 1u );
			facePipeline.descriptorSet->createBinding( m_descriptorLayout->getBinding( 1u )
				, *m_view
				, *m_sampler );
			facePipeline.descriptorSet->update();
			++face;
		}
	}

	void EquirectangularToCube::render( ashes::CommandBuffer & commandBuffer )
	{
		for ( auto & facePipeline : m_faces )
		{
			commandBuffer.memoryBarrier( VkPipelineStageFlagBits::eTransfer
				, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
				, facePipeline.frameBuffer->begin()->getView().makeColourAttachment( VK_IMAGE_LAYOUT_UNDEFINED
					, 0u ) );
			commandBuffer.beginRenderPass( *m_renderPass
				, *facePipeline.frameBuffer
				, { VkClearColorValue{ 0, 0, 0, 0 } }
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
		m_commandBuffer->begin( VkCommandBufferUsageFlagBits::eOneTimeSubmit );
		render( *m_commandBuffer );
		m_commandBuffer->end();
		m_queue.submit( *m_commandBuffer, nullptr );
		m_queue.waitIdle();
	}
}
