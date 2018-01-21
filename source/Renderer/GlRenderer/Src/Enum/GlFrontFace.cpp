#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	GLenum convert( renderer::FrontFace const & value )
	{
		switch ( value )
		{
		case renderer::FrontFace::eCounterClockwise:
			return GL_CW;

		case renderer::FrontFace::eClockwise:
			return GL_CCW;

		default:
			assert( false && "Unsupported front face mode");
			return GL_CCW;
		}
	}
}
