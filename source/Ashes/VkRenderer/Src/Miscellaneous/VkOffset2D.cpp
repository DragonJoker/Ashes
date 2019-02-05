#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkOffset2D convert( ashes::Offset2D const & value )
	{
		return VkOffset2D
		{
			value.x,
			value.y,
		};
	}

	ashes::Offset2D convert( VkOffset2D const & value )
	{
		return ashes::Offset2D
		{
			value.x,
			value.y,
		};
	}
}
