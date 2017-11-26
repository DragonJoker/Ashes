#include "PolygonMode.hpp"

namespace renderer
{
	VkPolygonMode convert( PolygonMode const & value )
	{
		switch ( value )
		{
		case PolygonMode::eFill:
			return VK_POLYGON_MODE_FILL;

		case PolygonMode::eLine:
			return VK_POLYGON_MODE_LINE;

		case PolygonMode::ePoint:
			return VK_POLYGON_MODE_POINT;

		default:
			assert( false && "Unsupported polygon mode" );
			return VK_POLYGON_MODE_FILL;
		}
	}
}
