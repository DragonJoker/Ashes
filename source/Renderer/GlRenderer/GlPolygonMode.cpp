#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	GLenum convert( renderer::PolygonMode const & value )
	{
		switch ( value )
		{
		case renderer::PolygonMode::eFill:
			return GL_FILL;

		case renderer::PolygonMode::eLine:
			return GL_LINE;

		case renderer::PolygonMode::ePoint:
			return GL_POINT;

		default:
			assert( false && "Unsupported polygon mode" );
			return GL_FILL;
		}
	}
}
