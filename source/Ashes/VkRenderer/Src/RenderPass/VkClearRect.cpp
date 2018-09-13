#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkClearRect convert( ashes::ClearRect const & value )
	{
		return VkClearRect
		{
			{
				convert( value.offset ),
				convert( value.extent )
			},
			value.baseArrayLayer,
			value.layerCount,
		};
	}
}
