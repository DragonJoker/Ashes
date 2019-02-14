#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkCommandBufferResetFlags convert( ashes::CommandBufferResetFlags const & flags )
	{
		VkCommandBufferResetFlags result{ 0 };

		if ( checkFlag( flags, ashes::CommandBufferResetFlag::eReleaseResources ) )
		{
			result |= VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT;
		}

		return result;
	}
}
