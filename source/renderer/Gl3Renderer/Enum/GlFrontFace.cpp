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

	GlFrontFace convert( VkFrontFace const & value )
	{
		switch ( value )
		{
		case VK_FRONT_FACE_COUNTER_CLOCKWISE:
			return GL_FRONT_FACE_CCW;

		case VK_FRONT_FACE_CLOCKWISE:
			return GL_FRONT_FACE_CW;

		default:
			assert( false && "Unsupported front face mode");
			return GL_FRONT_FACE_CCW;
		}
	}
}
