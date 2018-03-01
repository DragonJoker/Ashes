#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkClearRect convert( renderer::ClearRect const & value )
	{
		return VkClearRect
		{
			{
				{
					value.offset[0],
					value.offset[1],
				},
				{
					value.extent[0],
					value.extent[1],
				}
			},
			value.baseArrayLayer,
			value.layerCount,
		};
	}
}
