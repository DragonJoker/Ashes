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
			value.size,
			value.alignment,
			value.memoryTypeBits,
			ashes::ResourceType( 0u ),
		};
	}
}
