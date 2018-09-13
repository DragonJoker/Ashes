#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkOffset3D convert( ashes::Offset3D const & value )
	{
		return VkOffset3D
		{
			value.x,
			value.y,
			value.z
		};
	}
}
