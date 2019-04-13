#include "AshesPP/Image/ImageView.hpp"

#include "AshesPP/Core/Device.hpp"
#include "AshesPP/Image/Image.hpp"

namespace ashes
{
	ImageView::ImageView( VkImageViewCreateInfo createInfo
		, VkImageView internal
		, Image const * image )
		: createInfo{ std::move( createInfo ) }
		, internal{ internal }
		, image{ image }
	{
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
			createInfo.image,
			createInfo.subresourceRange
		};
	}
}
