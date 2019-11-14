#include "GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	std::string getName( GlPolygonMode value )
	{
		switch ( value )
		{
		case GL_POLYGON_MODE_FILL:
			return "GL_FILL";

		case GL_POLYGON_MODE_LINE:
			return "GL_LINE";

		case GL_POLYGON_MODE_POINT:
			return "GL_POINT";

		default:
			assert( false && "Unsupported GlPolygonMode" );
			return "GlPolygonMode_UNKNOWN";
		}
	}

	GlPolygonMode convert( VkPolygonMode const & value )
	{
		switch ( value )
		{
		case VK_POLYGON_MODE_FILL:
			return GL_POLYGON_MODE_FILL;

		case VK_POLYGON_MODE_LINE:
			return GL_POLYGON_MODE_LINE;

		case VK_POLYGON_MODE_POINT:
			return GL_POLYGON_MODE_POINT;

		default:
			assert( false && "Unsupported polygon mode" );
			return GL_POLYGON_MODE_FILL;
		}
	}
}
