#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkFenceCreateFlags convert( ashes::FenceCreateFlags const & flags )
	{
		VkFenceCreateFlags result{ 0 };

		if ( checkFlag( flags, ashes::FenceCreateFlag::eSignaled ) )
		{
			result |= VK_FENCE_CREATE_SIGNALED_BIT;
		}

		return result;
	}
}
