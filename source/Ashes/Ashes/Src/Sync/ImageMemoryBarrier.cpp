/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/ImageMemoryBarrier.hpp"

#include "Image/ImageSubresourceRange.hpp"
#include "Image/Texture.hpp"

namespace ashes
{
	ImageMemoryBarrier::ImageMemoryBarrier( AccessFlags srcAccessMask
		, AccessFlags dstAccessMask
		, ImageLayout oldLayout
		, ImageLayout newLayout
		, uint32_t srcQueueFamilyIndex
		, uint32_t dstQueueFamilyIndex
		, Texture const & image
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
