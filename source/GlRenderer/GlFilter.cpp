#include "GlFilter.hpp"

namespace gl_renderer
{
	GLenum convert( renderer::Filter const & filter )
	{
		switch ( filter )
		{
		case renderer::Filter::eNearest:
			return GL_NEAREST;

		case renderer::Filter::eLinear:
			return GL_LINEAR;

		default:
			assert( false );
			return GL_NEAREST;
		}
	}

}
