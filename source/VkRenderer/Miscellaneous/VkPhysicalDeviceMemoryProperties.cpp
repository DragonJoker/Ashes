#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	ashes::PhysicalDeviceMemoryProperties convert( VkPhysicalDeviceMemoryProperties const & value )
	{
		ashes::PhysicalDeviceMemoryProperties result;

		for ( uint32_t i = 0u; i < value.memoryHeapCount; ++i )
		{
			result.memoryHeaps.push_back(
				{
					value.memoryHeaps[i].size,
					value.memoryHeaps[i].flags,
				} );
		}

		for ( uint32_t i = 0u; i < value.memoryTypeCount; ++i )
		{
			result.memoryTypes.push_back(
				{
					value.memoryTypes[i].propertyFlags,
					value.memoryTypes[i].heapIndex,
				} );
		}

		return result;
	}
}
