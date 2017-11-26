#include "CommandPoolCreateFlag.hpp"

namespace renderer
{
	VkCommandPoolCreateFlags convert( CommandPoolCreateFlags const & flags )
	{
		VkCommandPoolCreateFlags result{ 0 };

		if ( checkFlag( flags, CommandPoolCreateFlag::eTransient ) )
		{
			result |= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
		}

		if ( checkFlag( flags, CommandPoolCreateFlag::eResetCommandBuffer ) )
		{
			result |= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		}

		return result;
	}
}
