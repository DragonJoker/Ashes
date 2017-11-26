#include "ColourComponentFlag.hpp"

namespace renderer
{
	VkColorComponentFlags convert( ColourComponentFlags const & flags )
	{
		VkColorComponentFlags result{ 0 };

		if ( checkFlag( flags, ColourComponentFlag::eR ) )
		{
			result |= VK_COLOR_COMPONENT_R_BIT;
		}

		if ( checkFlag( flags, ColourComponentFlag::eG ) )
		{
			result |= VK_COLOR_COMPONENT_G_BIT;
		}

		if ( checkFlag( flags, ColourComponentFlag::eB ) )
		{
			result |= VK_COLOR_COMPONENT_B_BIT;
		}

		if ( checkFlag( flags, ColourComponentFlag::eA ) )
		{
			result |= VK_COLOR_COMPONENT_A_BIT;
		}

		return result;
	}
}
