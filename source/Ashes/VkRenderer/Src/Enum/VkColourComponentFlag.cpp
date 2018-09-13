#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkColorComponentFlags convert( ashes::ColourComponentFlags const & flags )
	{
		VkColorComponentFlags result{ 0 };

		if ( checkFlag( flags, ashes::ColourComponentFlag::eR ) )
		{
			result |= VK_COLOR_COMPONENT_R_BIT;
		}

		if ( checkFlag( flags, ashes::ColourComponentFlag::eG ) )
		{
			result |= VK_COLOR_COMPONENT_G_BIT;
		}

		if ( checkFlag( flags, ashes::ColourComponentFlag::eB ) )
		{
			result |= VK_COLOR_COMPONENT_B_BIT;
		}

		if ( checkFlag( flags, ashes::ColourComponentFlag::eA ) )
		{
			result |= VK_COLOR_COMPONENT_A_BIT;
		}

		return result;
	}
}
