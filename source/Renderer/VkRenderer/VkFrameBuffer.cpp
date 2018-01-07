/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkFrameBuffer.hpp"

#include "VkDevice.hpp"
#include "VkQueue.hpp"
#include "VkRenderPass.hpp"
#include "VkTexture.hpp"
#include "VkTextureView.hpp"

namespace vk_renderer
{
	namespace
	{
		TextureViewCRefArray convert( renderer::TextureCRefArray const & textures )
		{
			TextureViewCRefArray result;
			result.reserve( textures.size() );

			for ( auto & texture : textures )
			{
				result.emplace_back( static_cast< Texture const & >( texture.get() ).getView() );
			}

			return result;
		}
	}

	FrameBuffer::FrameBuffer( Device const & device
		, RenderPass const & renderPass
		, utils::IVec2 const & dimensions
		, renderer::TextureCRefArray const & textures )
		: renderer::FrameBuffer{ renderPass, dimensions, textures }
		, m_device{ device }
		, m_views{ convert( textures ) }
	{
		auto attachments = makeVkArray< VkImageView >( m_views );

		VkFramebufferCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			nullptr,
			0u,                                                 // flags
			renderPass,                                         // renderPass
			static_cast< uint32_t >( attachments.size() ),      // attachmentCount
			attachments.data(),                                 // pAttachments
			uint32_t( dimensions.x ),                           // width
			uint32_t( dimensions.y ),                           // height
			1u                                                  // layers
		};
		DEBUG_DUMP( createInfo );
		auto res = CreateFramebuffer( device
			, &createInfo
			, nullptr
			, &m_framebuffer );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "Frame buffer creation failed: " + getLastError() };
		}
	}

	FrameBuffer::~FrameBuffer()
	{
		if ( m_framebuffer )
		{
			DestroyFramebuffer( m_device, m_framebuffer, nullptr );
		}
	}

	void FrameBuffer::download( renderer::Queue const & queue
		, uint32_t index
		, uint32_t xoffset
		, uint32_t yoffset
		, uint32_t width
		, uint32_t height
		, utils::PixelFormat format
		, uint8_t * data )const noexcept
	{
		auto & attachImage = m_views[index].get().getImage();
		// Create the linear tiled destination image to copy to and to read the memory from
		Texture image{ m_device
			, format
			, utils::IVec2{ width, height }
			, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			, VK_IMAGE_USAGE_TRANSFER_DST_BIT
			, VK_IMAGE_TILING_LINEAR };

		// Do the actual blit from the swapchain image to our host visible destination image
		CommandBuffer copyCmd{ m_device, m_device.getGraphicsCommandPool() };
		copyCmd.begin();
		copyCmd.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, image.makeTransferDestination() );
		copyCmd.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT 
			, attachImage.makeTransferSource() );

		VkImageCopy imageCopyRegion{};
		imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageCopyRegion.srcSubresource.layerCount = 1;
		imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageCopyRegion.dstSubresource.layerCount = 1;
		imageCopyRegion.extent.width = width;
		imageCopyRegion.extent.height = height;
		imageCopyRegion.extent.depth = 1;
		imageCopyRegion.srcOffset.x = xoffset;
		imageCopyRegion.srcOffset.y = yoffset;
		imageCopyRegion.srcOffset.z = 0u;

		copyCmd.copyImage( imageCopyRegion, attachImage, image );
		copyCmd.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, image.makeGeneralLayout( VK_ACCESS_MEMORY_READ_BIT ) );

		copyCmd.end();

		{
			Fence fence{ m_device };
			queue.submit( copyCmd, &fence );
		}

		auto mapped = image.lock( 0u
			, uint32_t( VK_WHOLE_SIZE )
			, 0u );

		if ( mapped.data )
		{
			std::memcpy( data, mapped.data, mapped.size );
			image.unlock( uint32_t( VK_WHOLE_SIZE ), false );
		}
		m_frameBuffer->download( static_cast< Queue const & >( queue ).getQueue()
			, index
			, xoffset
			, yoffset
			, width
			, height
			, convert( format )
			, data );
	}
}
