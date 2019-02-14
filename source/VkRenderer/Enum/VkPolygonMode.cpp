#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkPolygonMode convert( ashes::PolygonMode const & value )
	{
		switch ( value )
		{
		case ashes::PolygonMode::eFill:
			return VK_POLYGON_MODE_FILL;

		case ashes::PolygonMode::eLine:
			return VK_POLYGON_MODE_LINE;

		case ashes::PolygonMode::ePoint:
			return VK_POLYGON_MODE_POINT;

		default:
			assert( false && "Unsupported polygon mode" );
			return VK_POLYGON_MODE_FILL;
		}
	}
}
