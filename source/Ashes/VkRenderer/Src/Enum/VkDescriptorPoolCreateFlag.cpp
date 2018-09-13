#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkDescriptorPoolCreateFlags convert( ashes::DescriptorPoolCreateFlags const & flags )
	{
		VkDescriptorPoolCreateFlags result{ 0 };

		if ( checkFlag( flags, ashes::DescriptorPoolCreateFlag::eFreeDescriptorSet ) )
		{
			result |= VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		}

		return result;
	}
}
