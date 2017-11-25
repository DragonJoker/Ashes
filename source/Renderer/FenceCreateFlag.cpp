#include "FenceCreateFlag.hpp"

namespace renderer
{
	VkFenceCreateFlags convert( FenceCreateFlags const & flags )
	{
		VkFenceCreateFlags result{ 0 };

		if ( checkFlag( flags, FenceCreateFlag::eSignaled ) )
		{
			result |= VK_FENCE_CREATE_SIGNALED_BIT;
		}

		return result;
	}
}
