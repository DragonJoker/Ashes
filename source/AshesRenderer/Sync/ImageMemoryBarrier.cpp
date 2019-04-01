/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesRenderer/Sync/ImageMemoryBarrier.hpp"

#include "AshesRenderer/Image/ImageSubresourceRange.hpp"
#include "AshesRenderer/Image/Image.hpp"

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
