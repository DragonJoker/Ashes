#include "CullModeFlag.hpp"

namespace renderer
{
	VkCullModeFlags convert( CullModeFlags const & flags )
	{
		VkCullModeFlags result{ 0 };

		if ( checkFlag( flags, CullModeFlag::eFront ) )
		{
			result |= VK_CULL_MODE_FRONT_BIT;
		}

		if ( checkFlag( flags, CullModeFlag::eBack ) )
		{
			result |= VK_CULL_MODE_BACK_BIT;
		}

		return result;
	}
}
