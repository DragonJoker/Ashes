#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkCullModeFlags convert( ashes::CullModeFlags const & flags )
	{
		VkCullModeFlags result{ 0 };

		if ( checkFlag( flags, ashes::CullModeFlag::eFront ) )
		{
			result |= VK_CULL_MODE_FRONT_BIT;
		}

		if ( checkFlag( flags, ashes::CullModeFlag::eBack ) )
		{
			result |= VK_CULL_MODE_BACK_BIT;
		}

		return result;
	}
}
