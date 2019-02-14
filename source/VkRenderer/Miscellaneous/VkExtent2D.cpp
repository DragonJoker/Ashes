#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkExtent2D convert( ashes::Extent2D const & value )
	{
		return VkExtent2D
		{
			value.width,
			value.height,
		};
	}

	ashes::Extent2D convert( VkExtent2D const & value )
	{
		return ashes::Extent2D
		{
			value.width,
			value.height,
		};
	}
}
