#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlFrontFace value )
	{
		switch ( value )
		{
		case gl_renderer::GL_FRONT_FACE_CW:
			return "GL_CW";

		case gl_renderer::GL_FRONT_FACE_CCW:
			return "GL_CCW";

		default:
			assert( false && "Unsupported GlFrontFace" );
			return "GlFrontFace_UNKNOWN";
		}
	}

	GlFrontFace convert( ashes::FrontFace const & value )
	{
		switch ( value )
		{
		case ashes::FrontFace::eCounterClockwise:
			return GL_FRONT_FACE_CCW;

		case ashes::FrontFace::eClockwise:
			return GL_FRONT_FACE_CW;

		default:
			assert( false && "Unsupported front face mode");
			return GL_FRONT_FACE_CCW;
		}
	}
}
