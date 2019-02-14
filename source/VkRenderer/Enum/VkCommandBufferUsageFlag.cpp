#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkCommandBufferUsageFlags convert( ashes::CommandBufferUsageFlags const & flags )
	{
		VkCommandBufferUsageFlags result{ 0 };

		if ( checkFlag( flags, ashes::CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			result |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		}

		if ( checkFlag( flags, ashes::CommandBufferUsageFlag::eRenderPassContinue ) )
		{
			result |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		}

		if ( checkFlag( flags, ashes::CommandBufferUsageFlag::eSimultaneousUse ) )
		{
			result |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		}

		return result;
	}
}
