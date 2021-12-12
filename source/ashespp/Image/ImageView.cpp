#include "ashespp/Image/ImageView.hpp"

#include "ashespp/Core/Device.hpp"
#include "ashespp/Image/Image.hpp"

namespace ashes
{
	ImageView::ImageView( VkImageViewCreateInfo pcreateInfo
		, VkImageView pinternal
		, Image const * pimage )
		: createInfo{ std::move( pcreateInfo ) }
		, internal{ pinternal }
		, image{ pimage }
	{
		createInfo.image = image ? *image : VkImage{};
	}

	VkImageMemoryBarrier ImageView::makeGeneralLayout( VkImageLayout srcLayout
		, VkAccessFlags dstAccessFlags
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return makeLayoutTransition( srcLayout
			, VK_IMAGE_LAYOUT_GENERAL
			, getAccessMask( srcLayout )
			, dstAccessFlags
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkImageMemoryBarrier ImageView::makeTransferDestination( VkImageLayout srcLayout
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return makeLayoutTransition( srcLayout
			, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkImageMemoryBarrier ImageView::makeTransferSource( VkImageLayout srcLayout
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return makeLayoutTransition( srcLayout
			, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkImageMemoryBarrier ImageView::makeShaderInputResource( VkImageLayout srcLayout
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return makeLayoutTransition( srcLayout
			, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkImageMemoryBarrier ImageView::makeDepthStencilReadOnly( VkImageLayout srcLayout
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return makeLayoutTransition( srcLayout
			, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkImageMemoryBarrier ImageView::makeColourAttachment( VkImageLayout srcLayout
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return makeLayoutTransition( srcLayout
			, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkImageMemoryBarrier ImageView::makeDepthStencilAttachment( VkImageLayout srcLayout
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return makeLayoutTransition( srcLayout
			, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkImageMemoryBarrier ImageView::makePresentSource( VkImageLayout srcLayout
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return makeLayoutTransition( srcLayout
			, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkImageMemoryBarrier ImageView::makeLayoutTransition( VkImageLayout srcLayout
		, VkImageLayout dstLayout
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return makeLayoutTransition( srcLayout
			, dstLayout
			, getAccessMask( srcLayout )
			, getAccessMask( dstLayout )
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkImageMemoryBarrier ImageView::makeLayoutTransition( VkImageLayout srcLayout
		, VkImageLayout dstLayout
		, VkAccessFlags srcAccessFlags
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return makeLayoutTransition( srcLayout
			, dstLayout
			, srcAccessFlags
			, getAccessMask( dstLayout )
			, srcQueueFamily
			, dstQueueFamily );
	}

	VkImageMemoryBarrier ImageView::makeLayoutTransition( VkImageLayout srcLayout
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
