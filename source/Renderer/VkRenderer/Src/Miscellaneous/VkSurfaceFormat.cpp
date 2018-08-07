#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkSurfaceFormatKHR convert( renderer::SurfaceFormat const & value )
	{
		return VkSurfaceFormatKHR
		{
			convert( value.format ),
			convert( value.colorSpace )
		};
	}

	renderer::SurfaceFormat convert( VkSurfaceFormatKHR const & value )
	{
		return renderer::SurfaceFormat
		{
			convert( value.format ),
			convertColorSpace( value.colorSpace )
		};
	}
}
