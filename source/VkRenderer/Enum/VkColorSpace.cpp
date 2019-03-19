#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkColorSpaceKHR convert( ashes::ColorSpace const & value )
	{
		return VkColorSpaceKHR( value );
	}

	ashes::ColorSpace convertColorSpace( VkColorSpaceKHR const & value )
	{
		return ashes::ColorSpace( value );
	}
}
