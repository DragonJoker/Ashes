#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkMemoryHeapFlags convert( ashes::MemoryHeapFlags const & flags )
	{
		VkMemoryHeapFlags result{ 0 };

		if ( checkFlag( flags, ashes::MemoryHeapFlag::eDeviceLocal ) )
		{
			result |= VK_MEMORY_HEAP_DEVICE_LOCAL_BIT;
		}

		return result;
	}

	ashes::MemoryHeapFlags convertMemoryHeapFlags( VkMemoryHeapFlags const & flags )
	{
		ashes::MemoryHeapFlags result;

		if ( ashes::checkFlag( flags, VK_MEMORY_HEAP_DEVICE_LOCAL_BIT ) )
		{
			result |= ashes::MemoryHeapFlag::eDeviceLocal;
		}

		return result;
	}
}
