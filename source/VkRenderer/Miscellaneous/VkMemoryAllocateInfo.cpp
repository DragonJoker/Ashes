#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkMemoryAllocateInfo convert( ashes::MemoryAllocateInfo const & value )
	{
		return VkMemoryAllocateInfo
		{
			VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			nullptr,
			value.allocationSize,
			value.memoryTypeIndex,
		};
	}
}
