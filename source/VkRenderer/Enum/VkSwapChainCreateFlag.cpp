#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkSwapchainCreateFlagsKHR convert( ashes::SwapChainCreateFlags const & flags )
	{
		return VkSwapchainCreateFlagsKHR( flags );
	}
}
