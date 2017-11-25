#include "CommandBufferResetFlag.hpp"

namespace renderer
{
	VkCommandBufferResetFlags convert( CommandBufferResetFlags const & flags )
	{
		VkCommandBufferResetFlags result{ 0 };

		if ( checkFlag( flags, CommandBufferResetFlag::eReleaseResources ) )
		{
			result |= VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT;
		}

		return result;
	}
}
