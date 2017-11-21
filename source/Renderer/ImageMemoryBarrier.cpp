#include "ImageMemoryBarrier.hpp"

#include "ImageSubresourceRange.hpp"
#include "Texture.hpp"

namespace renderer
{
	ImageMemoryBarrier::ImageMemoryBarrier( AccessFlags srcAccessMask
		, AccessFlags dstAccessMask
		, ImageLayout oldLayout
		, ImageLayout newLayout
		, uint32_t srcQueueFamilyIndex
		, uint32_t dstQueueFamilyIndex
		, Texture const & image
		, ImageSubresourceRange const & subresourceRange )
		: m_barrier
	{
			VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			nullptr,
			convert( srcAccessMask ),
			convert( dstAccessMask ),
			convert( oldLayout ),
			convert( newLayout ),
			srcQueueFamilyIndex,
			dstQueueFamilyIndex,
			image.getImage(),
			subresourceRange.getRange()
		}
	{
	}

	ImageMemoryBarrier::ImageMemoryBarrier( VkImageMemoryBarrier const & barrier )
		: m_barrier{ barrier }
	{
	}
}
