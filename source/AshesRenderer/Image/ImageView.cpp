#include "AshesRenderer/Image/ImageView.hpp"

#include "AshesRenderer/Core/Device.hpp"
#include "AshesRenderer/Sync/ImageMemoryBarrier.hpp"

namespace ashes
{
	ImageView::ImageView( Device const & device
		, Image const & image
		, ImageViewCreateInfo const & createInfo )
		: m_device{ device }
		, m_image{ image }
		, m_createInfo{ createInfo }
	{
		registerObject( m_device, "ImageView", this );
	}

	ImageView::~ImageView()
	{
		unregisterObject( m_device, this );
	}

	ImageMemoryBarrier ImageView::makeGeneralLayout( ImageLayout srcLayout
		, AccessFlags srcAccessFlags
		, AccessFlags dstAccessFlags
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return doMakeLayoutTransition( srcLayout
			, ImageLayout::eGeneral
			, srcAccessFlags
			, dstAccessFlags
			, srcQueueFamily
			, dstQueueFamily );
	}

	ImageMemoryBarrier ImageView::makeTransferDestination( ImageLayout srcLayout
		, AccessFlags srcAccessFlags
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return doMakeLayoutTransition( srcLayout
			, ImageLayout::eTransferDstOptimal
			, srcAccessFlags
			, AccessFlag::eTransferWrite
			, srcQueueFamily
			, dstQueueFamily );
	}

	ImageMemoryBarrier ImageView::makeTransferSource( ImageLayout srcLayout
		, AccessFlags srcAccessFlags
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return doMakeLayoutTransition( srcLayout
			, ImageLayout::eTransferSrcOptimal
			, srcAccessFlags
			, AccessFlag::eTransferRead
			, srcQueueFamily
			, dstQueueFamily );
	}

	ImageMemoryBarrier ImageView::makeShaderInputResource( ImageLayout srcLayout
		, AccessFlags srcAccessFlags
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return doMakeLayoutTransition( srcLayout
			, ImageLayout::eShaderReadOnlyOptimal
			, srcAccessFlags
			, AccessFlag::eShaderRead
			, srcQueueFamily
			, dstQueueFamily );
	}

	ImageMemoryBarrier ImageView::makeDepthStencilReadOnly( ImageLayout srcLayout
		, AccessFlags srcAccessFlags
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return doMakeLayoutTransition( srcLayout
			, ImageLayout::eDepthStencilReadOnlyOptimal
			, srcAccessFlags
			, AccessFlag::eShaderRead
			, srcQueueFamily
			, dstQueueFamily );
	}

	ImageMemoryBarrier ImageView::makeColourAttachment( ImageLayout srcLayout
		, AccessFlags srcAccessFlags
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return doMakeLayoutTransition( srcLayout
			, ImageLayout::eColourAttachmentOptimal
			, srcAccessFlags
			, AccessFlag::eColourAttachmentWrite
			, srcQueueFamily
			, dstQueueFamily );
	}

	ImageMemoryBarrier ImageView::makeDepthStencilAttachment( ImageLayout srcLayout
		, AccessFlags srcAccessFlags
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return doMakeLayoutTransition( srcLayout
			, ImageLayout::eDepthStencilAttachmentOptimal
			, srcAccessFlags
			, AccessFlag::eDepthStencilAttachmentWrite
			, srcQueueFamily
			, dstQueueFamily );
	}

	ImageMemoryBarrier ImageView::makePresentSource( ImageLayout srcLayout
		, AccessFlags srcAccessFlags
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return doMakeLayoutTransition( srcLayout
			, ImageLayout::ePresentSrc
			, srcAccessFlags
			, AccessFlag::eMemoryRead
			, srcQueueFamily
			, dstQueueFamily );
	}

	ashes::ImageMemoryBarrier ImageView::doMakeLayoutTransition( ashes::ImageLayout srcLayout
		, ashes::ImageLayout dstLayout
		, ashes::AccessFlags srcAccessFlags
		, ashes::AccessFlags dstAccessMask
		, uint32_t srcQueueFamily
		, uint32_t dstQueueFamily )const
	{
		return ashes::ImageMemoryBarrier
		{
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
