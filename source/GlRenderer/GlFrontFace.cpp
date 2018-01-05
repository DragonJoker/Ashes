#include "GlFrontFace.hpp"

namespace gl_renderer
{
	GLenum convert( renderer::FrontFace const & value )
	{
		switch ( value )
		{
		case renderer::FrontFace::eCounterClockwise:
			return GL_CCW;

		case renderer::FrontFace::eClockwise:
			return GL_CW;

		default:
			assert( false && "Unsupported front face mode");
			return GL_CCW;
		}
	}
}
