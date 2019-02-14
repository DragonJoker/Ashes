#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkDependencyFlags convert( ashes::DependencyFlags const & flags )
	{
		VkDependencyFlags result{ 0 };

		if ( checkFlag( flags, ashes::DependencyFlag::eByRegion ) )
		{
			result |= VK_DEPENDENCY_BY_REGION_BIT;
		}

		return result;
	}

	ashes::DependencyFlags convertDependencyFlags( VkDependencyFlags const & flags )
	{
		ashes::DependencyFlags result;

		if ( ashes::checkFlag( flags, VK_DEPENDENCY_BY_REGION_BIT ) )
		{
			result |= ashes::DependencyFlag::eByRegion;
		}

		return result;
	}
}
