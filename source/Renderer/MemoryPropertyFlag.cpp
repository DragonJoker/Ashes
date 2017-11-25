#include "MemoryPropertyFlag.hpp"

namespace renderer
{
	VkMemoryPropertyFlags convert( MemoryPropertyFlags const & flags )
	{
		VkMemoryPropertyFlags result{ 0 };

		if ( checkFlag( flags, MemoryPropertyFlag::eDeviceLocal ) )
		{
			result |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		}

		if ( checkFlag( flags, MemoryPropertyFlag::eHostCached ) )
		{
			result |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
		}

		if ( checkFlag( flags, MemoryPropertyFlag::eHostCoherent ) )
		{
			result |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		}

		if ( checkFlag( flags, MemoryPropertyFlag::eHostVisible ) )
		{
			result |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		}

		if ( checkFlag( flags, MemoryPropertyFlag::eLazilyAllocated ) )
		{
			result |= VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
		}

		return result;
	}
}
