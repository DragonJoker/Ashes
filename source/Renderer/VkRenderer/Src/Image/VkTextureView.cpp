#include "Image/VkTextureView.hpp"

#include "Command/VkCommandBuffer.hpp"
#include "Core/VkDevice.hpp"
#include "Sync/VkImageMemoryBarrier.hpp"
#include "Image/VkComponentMapping.hpp"
#include "Image/VkImageSubresourceRange.hpp"
#include "Image/VkTexture.hpp"

namespace vk_renderer
{
	TextureView::TextureView( Device const & device
		, Texture const & image
		, renderer::ImageViewCreateInfo const & createInfo )
		: renderer::TextureView{ device
			, image
			, createInfo }
		, m_device{ device }
	{
		VkImageViewCreateInfo vkcreateInfo
		{
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			nullptr,
			0,
			static_cast< Texture const & >( getTexture() ),
			convert( createInfo.viewType ),
			convert( createInfo.format ),
			convert( createInfo.components ),
			convert( createInfo.subresourceRange )
		};
		DEBUG_DUMP( vkcreateInfo );
		auto res = m_device.vkCreateImageView( m_device
			, &vkcreateInfo
			, nullptr
			, &m_view );
		checkError( res, "ImageView creation" );
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
}
