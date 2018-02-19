#include "Image/TextureView.hpp"

#include "Sync/ImageMemoryBarrier.hpp"

namespace renderer
{
	TextureView::TextureView( Device const & device
		, Texture const & image
		, TextureType type
		, PixelFormat format
		, uint32_t baseMipLevel
		, uint32_t levelCount
		, uint32_t baseArrayLayer
		, uint32_t layerCount
		, ComponentMapping const & mapping )
		: m_device{ device }
		, m_format{ format }
		, m_image{ image }
		, m_type{ type }
		, m_subResourceRange
		{
			getAspectMask( m_format ),
			baseMipLevel,
			levelCount,
			baseArrayLayer,
			layerCount
		}
	{
	}

	ImageMemoryBarrier TextureView::makeGeneralLayout( ImageLayout srcLayout
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

	ImageMemoryBarrier TextureView::makeTransferDestination( ImageLayout srcLayout
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

	ImageMemoryBarrier TextureView::makeTransferSource( ImageLayout srcLayout
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

	ImageMemoryBarrier TextureView::makeShaderInputResource( ImageLayout srcLayout
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

	ImageMemoryBarrier TextureView::makeDepthStencilReadOnly( ImageLayout srcLayout
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

	ImageMemoryBarrier TextureView::makeColourAttachment( ImageLayout srcLayout
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

	ImageMemoryBarrier TextureView::makeDepthStencilAttachment( ImageLayout srcLayout
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

	ImageMemoryBarrier TextureView::makePresentSource( ImageLayout srcLayout
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
}
