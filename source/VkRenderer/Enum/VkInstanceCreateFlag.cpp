#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkInstanceCreateFlags convert( ashes::InstanceCreateFlags const & flags )
	{
		return VkInstanceCreateFlags( flags );
	}
}
