#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkMemoryRequirements convert( ashes::MemoryRequirements const & value )
	{
		return VkMemoryRequirements
		{
			value.size,
			value.alignment,
			value.memoryTypeBits,
		};
	}

	ashes::MemoryRequirements convert( VkMemoryRequirements const & value )
	{
		return ashes::MemoryRequirements
		{
			ashes::ResourceType( 0u ),
			value.size,
			value.alignment,
			value.memoryTypeBits,
		};
	}
}
