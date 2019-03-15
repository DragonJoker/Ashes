#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkDeviceQueueCreateFlags convert( ashes::DeviceQueueCreateFlags const & flags )
	{
		VkDeviceQueueCreateFlags result{ 0 };

		if ( checkFlag( flags, ashes::DeviceQueueCreateFlag::eProtected ) )
		{
			result |= VK_DEVICE_QUEUE_CREATE_PROTECTED_BIT;
		}

		return result;
	}
}
