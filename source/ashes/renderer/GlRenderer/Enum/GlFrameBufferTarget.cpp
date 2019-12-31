#include "GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	std::string getName( GlFrameBufferTarget value )
	{
		switch ( value )
		{
		case GL_FRAMEBUFFER:
			return "GL_FRAMEBUFFER";

		case GL_READ_FRAMEBUFFER:
			return "GL_READ_FRAMEBUFFER";

		case GL_DRAW_FRAMEBUFFER:
			return "GL_DRAW_FRAMEBUFFER";

		default:
			assert( false && "Unsupported GlFrameBufferTarget" );
			return "GlFrameBufferTarget_UNKNOWN";
		}
	}
}
