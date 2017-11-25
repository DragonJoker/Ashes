#include "CommandBufferUsageFlag.hpp"

namespace renderer
{
	VkCommandBufferUsageFlags convert( CommandBufferUsageFlags const & flags )
	{
		VkCommandBufferUsageFlags result{ 0 };

		if ( checkFlag( flags, CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			result |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		}

		if ( checkFlag( flags, CommandBufferUsageFlag::eRenderPassContinue ) )
		{
			result |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		}

		if ( checkFlag( flags, CommandBufferUsageFlag::eSimultaneousUse ) )
		{
			result |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		}

		return result;
	}
}
