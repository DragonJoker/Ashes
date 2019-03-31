#include "VkRendererPrerequisites.hpp"

#include "Image/VkImage.hpp"

namespace vk_renderer
{
	VkImageMemoryBarrier convert( ashes::ImageMemoryBarrier const & barrier )
	{
		return VkImageMemoryBarrier
		{
			VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			nullptr,
			convert( barrier.srcAccessMask ),
			convert( barrier.dstAccessMask ),
			convert( barrier.oldLayout ),
			convert( barrier.newLayout ),
			barrier.srcQueueFamilyIndex,
			barrier.dstQueueFamilyIndex,
			static_cast< Image const & >( barrier.image ),
			convert( barrier.subresourceRange )
		};
	}
}
