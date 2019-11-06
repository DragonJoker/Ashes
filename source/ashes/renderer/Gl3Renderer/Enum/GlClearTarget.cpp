#include "GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	std::string getName( GlClearTarget value )
	{
		switch ( value )
		{
		case GL_CLEAR_TARGET_COLOR:
			return "GL_COLOR";

		case GL_CLEAR_TARGET_DEPTH:
			return " GL_DEPTH";

		case GL_CLEAR_TARGET_STENCIL:
			return "GL_STENCIL";

		case GL_CLEAR_TARGET_DEPTH_STENCIL:
			return "GL_DEPTH_STENCIL";

		default:
			assert( false && "Unsupported GlClearTarget" );
			return "GlClearTarget_UNKNOWN";
		}
	}
}
