#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkExtent2D convert( renderer::Extent2D const & value )
	{
		return VkExtent2D
		{
			value.width,
			value.height,
		};
	}

	renderer::Extent2D convert( VkExtent2D const & value )
	{
		return renderer::Extent2D
		{
			value.width,
			value.height,
		};
	}
}
