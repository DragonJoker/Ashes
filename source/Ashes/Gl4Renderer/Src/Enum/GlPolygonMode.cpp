#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlPolygonMode value )
	{
		switch ( value )
		{
		case gl_renderer::GL_POLYGON_MODE_FILL:
			return "GL_FILL";

		case gl_renderer::GL_POLYGON_MODE_LINE:
			return "GL_LINE";

		case gl_renderer::GL_POLYGON_MODE_POINT:
			return "GL_POINT";

		default:
			assert( false && "Unsupported GlPolygonMode" );
			return "GlPolygonMode_UNKNOWN";
		}
	}

	GlPolygonMode convert( ashes::PolygonMode const & value )
	{
		switch ( value )
		{
		case ashes::PolygonMode::eFill:
			return GL_POLYGON_MODE_FILL;

		case ashes::PolygonMode::eLine:
			return GL_POLYGON_MODE_LINE;

		case ashes::PolygonMode::ePoint:
			return GL_POLYGON_MODE_POINT;

		default:
			assert( false && "Unsupported polygon mode" );
			return GL_POLYGON_MODE_FILL;
		}
	}
}
