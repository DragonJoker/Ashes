#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkMemoryPropertyFlags convert( ashes::MemoryPropertyFlags const & flags )
	{
		VkMemoryPropertyFlags result{ 0 };

		if ( checkFlag( flags, ashes::MemoryPropertyFlag::eDeviceLocal ) )
		{
			result |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		}

		if ( checkFlag( flags, ashes::MemoryPropertyFlag::eHostCached ) )
		{
			result |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
		}

		if ( checkFlag( flags, ashes::MemoryPropertyFlag::eHostCoherent ) )
		{
			result |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		}

		if ( checkFlag( flags, ashes::MemoryPropertyFlag::eHostVisible ) )
		{
			result |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		}

		if ( checkFlag( flags, ashes::MemoryPropertyFlag::eLazilyAllocated ) )
		{
			result |= VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
		}

		return result;
	}
}
