#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkDeviceQueueCreateInfo convert( ashes::DeviceQueueCreateInfo const & value )
	{
		return VkDeviceQueueCreateInfo
		{
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			nullptr,
			convert( value.flags ),
			value.queueFamilyIndex,
			uint32_t( value.queuePriorities.size() ),
			value.queuePriorities.data(),
		};
	}
}
