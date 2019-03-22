#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	ashes::QueueFamilyProperties convert( VkQueueFamilyProperties const & value )
	{
		return ashes::QueueFamilyProperties
		{
			convertQueueFlags( value.queueFlags ),
			value.queueCount,
			value.timestampValidBits,
			{
				value.minImageTransferGranularity.width,
				value.minImageTransferGranularity.height,
				value.minImageTransferGranularity.depth,
			}
		};
	}
}
