#include "AshesPP/Image/ImageView.hpp"

#include "AshesPP/Core/Device.hpp"
#include "AshesPP/Image/Image.hpp"

namespace ashespp
{
	ImageView::ImageView( Device const & device
		, Image const & image
		, VkImageViewCreateInfo const & createInfo )
		: m_device{ device }
		, m_image{ image }
		, m_createInfo{ createInfo }
	{
		VkImageViewCreateInfo vkcreateInfo
		{
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			nullptr,
			0,
			getImage(),
			createInfo.viewType,
			createInfo.format,
			createInfo.components,
			createInfo.subresourceRange
		};
		DEBUG_DUMP( vkcreateInfo );
		auto res = m_device.vkCreateImageView( m_device
			, &vkcreateInfo
			, nullptr
			, &m_internal );
		checkError( res, "ImageView creation" );
		registerObject( m_device, "ImageView", this );
	}

	ImageView::~ImageView()
	{
		unregisterObject( m_device, this );

		if ( m_internal )
		{
			m_device.vkDestroyImageView( m_device
				, m_internal
				, nullptr );
		}
	}

	VkImageMemoryBarrier ImageView::makeGeneralLayout( VkImageLayout srcLayout
		, VkAccessFlags srcAccessFlags
		, VkAccessFlags dstAccessFlags
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return doMakeLayoutTransition( srcLayout
			, VkImageLayout::VK_IMAGE_LAYOUT_GENERAL
			, srcAccessFlags
			, dstAccessFlags
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkImageMemoryBarrier ImageView::makeTransferDestination( VkImageLayout srcLayout
		, VkAccessFlags srcAccessFlags
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return doMakeLayoutTransition( srcLayout
			, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
			, srcAccessFlags
			, VkAccessFlagBits::VK_ACCESS_TRANSFER_WRITE_BIT
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkImageMemoryBarrier ImageView::makeTransferSource( VkImageLayout srcLayout
		, VkAccessFlags srcAccessFlags
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return doMakeLayoutTransition( srcLayout
			, VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
			, srcAccessFlags
			, VkAccessFlagBits::VK_ACCESS_TRANSFER_READ_BIT
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkImageMemoryBarrier ImageView::makeShaderInputResource( VkImageLayout srcLayout
		, VkAccessFlags srcAccessFlags
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return doMakeLayoutTransition( srcLayout
			, VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			, srcAccessFlags
			, VkAccessFlagBits::VK_ACCESS_SHADER_READ_BIT
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkImageMemoryBarrier ImageView::makeDepthStencilReadOnly( VkImageLayout srcLayout
		, VkAccessFlags srcAccessFlags
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return doMakeLayoutTransition( srcLayout
			, VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
			, srcAccessFlags
			, VkAccessFlagBits::VK_ACCESS_SHADER_READ_BIT
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkImageMemoryBarrier ImageView::makeColourAttachment( VkImageLayout srcLayout
		, VkAccessFlags srcAccessFlags
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return doMakeLayoutTransition( srcLayout
			, VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
			, srcAccessFlags
			, VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkImageMemoryBarrier ImageView::makeDepthStencilAttachment( VkImageLayout srcLayout
		, VkAccessFlags srcAccessFlags
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return doMakeLayoutTransition( srcLayout
			, VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
			, srcAccessFlags
			, VkAccessFlagBits::VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkImageMemoryBarrier ImageView::makePresentSource( VkImageLayout srcLayout
		, VkAccessFlags srcAccessFlags
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return doMakeLayoutTransition( srcLayout
			, VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
			, srcAccessFlags
			, VkAccessFlagBits::VK_ACCESS_MEMORY_READ_BIT
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkImageMemoryBarrier ImageView::doMakeLayoutTransition( VkImageLayout srcLayout
		, VkImageLayout dstLayout
		, VkAccessFlags srcAccessFlags
		, VkAccessFlags dstAccessMask
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return VkImageMemoryBarrier
		{
			VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			nullptr,
			srcAccessFlags,
			dstAccessMask,
			srcLayout,
			dstLayout,
			srcQueueFamily,
			dstQueueFamily,
			getImage(),
			getSubResourceRange()
		};
	}
}
