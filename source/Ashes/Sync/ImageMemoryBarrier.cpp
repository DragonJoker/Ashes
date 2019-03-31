/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Sync/ImageMemoryBarrier.hpp"

#include "Ashes/Image/ImageSubresourceRange.hpp"
#include "Ashes/Image/Image.hpp"

namespace ashes
{
	ImageMemoryBarrier::ImageMemoryBarrier( AccessFlags srcAccessMask
		, AccessFlags dstAccessMask
		, ImageLayout oldLayout
		, ImageLayout newLayout
		, uint32_t srcQueueFamilyIndex
		, uint32_t dstQueueFamilyIndex
		, Image const & image
		, ImageSubresourceRange const & subresourceRange )
		: srcAccessMask{ srcAccessMask }
		, dstAccessMask{ dstAccessMask }
		, oldLayout{ oldLayout }
		, newLayout{ newLayout }
		, srcQueueFamilyIndex{ srcQueueFamilyIndex }
		, dstQueueFamilyIndex{ dstQueueFamilyIndex }
		, image{ image }
		, subresourceRange{ subresourceRange }
	{
	}
}
