#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkSurfaceFormatKHR convert( ashes::SurfaceFormat const & value )
	{
		return VkSurfaceFormatKHR
		{
			convert( value.format ),
			convert( value.colorSpace )
		};
	}

	ashes::SurfaceFormat convert( VkSurfaceFormatKHR const & value )
	{
		return ashes::SurfaceFormat
		{
			convert( value.format ),
			convertColorSpace( value.colorSpace )
		};
	}
}
