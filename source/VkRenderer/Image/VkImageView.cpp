#include "Image/VkImageView.hpp"

#include "Command/VkCommandBuffer.hpp"
#include "Core/VkDevice.hpp"
#include "Sync/VkImageMemoryBarrier.hpp"
#include "Image/VkComponentMapping.hpp"
#include "Image/VkImageSubresourceRange.hpp"
#include "Image/VkImage.hpp"

namespace vk_renderer
{
	ImageView::ImageView( Device const & device
		, Image const & image
		, ashes::ImageViewCreateInfo const & createInfo )
		: ashes::ImageView{ device
			, image
			, createInfo }
		, m_device{ device }
	{
		VkImageViewCreateInfo vkcreateInfo
		{
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			nullptr,
			0,
			static_cast< Image const & >( getImage() ),
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

	ImageView::~ImageView()
	{
		if ( m_view )
		{
			m_device.vkDestroyImageView( m_device
				, m_view
				, nullptr );
		}
	}
}
