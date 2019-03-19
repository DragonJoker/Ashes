#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkDeviceCreateFlags convert( ashes::DeviceCreateFlags const & flags )
	{
		return VkDeviceCreateFlags( flags );
	}
}
