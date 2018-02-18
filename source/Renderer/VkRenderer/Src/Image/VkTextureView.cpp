#include "Image/VkTextureView.hpp"

#include "Command/VkCommandBuffer.hpp"
#include "Core/VkDevice.hpp"
#include "Sync/VkImageMemoryBarrier.hpp"
#include "Image/VkComponentMapping.hpp"
#include "Image/VkImageSubresourceRange.hpp"
#include "Core/VkRenderingResources.hpp"
#include "Image/VkTexture.hpp"

namespace vk_renderer
{
	TextureView::TextureView( Device const & device
		, Texture const & image
		, renderer::TextureType type
		, renderer::PixelFormat format
		, uint32_t baseMipLevel
		, uint32_t levelCount
		, uint32_t baseArrayLayer
		, uint32_t layerCount
		, renderer::ComponentMapping const & mapping )
		: renderer::TextureView{ device
			, image
			, type
			, format
			, baseMipLevel
			, levelCount
			, baseArrayLayer
			, layerCount
			, mapping }
		, m_device{ device }
	{
		VkImageViewCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			nullptr,
			0,
			static_cast< Texture const & >( getTexture() ),
			convert( type ),
			convert( getFormat() ),
			convert( mapping ),
			convert( getSubResourceRange() )
		};
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateImageView( m_device
			, &createInfo
			, nullptr
			, &m_view );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "Image view creation failed: " + getLastError() };
		}
	}

	TextureView::~TextureView()
	{
		if ( m_view )
		{
			m_device.vkDestroyImageView( m_device
				, m_view
				, nullptr );
		}
	}

	renderer::ImageMemoryBarrier TextureView::doMakeLayoutTransition( renderer::ImageLayout srcLayout
		, renderer::ImageLayout dstLayout
		, renderer::AccessFlags srcAccessFlags
		, renderer::AccessFlags dstAccessMask
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		renderer::ImageMemoryBarrier transitionBarrier
		{
			srcAccessFlags,
			dstAccessMask,
			srcLayout,
			dstLayout,
			srcQueueFamily,
			dstQueueFamily,
			getTexture(),
			getSubResourceRange()
		};
		DEBUG_DUMP( convert( transitionBarrier ) );
		return transitionBarrier;
	}
}
