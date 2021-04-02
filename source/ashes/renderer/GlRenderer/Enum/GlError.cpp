#include "GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	std::string getName( GlError error )
	{
		switch ( error )
		{
		case GL_SUCCESS:
			return "GL_SUCCESS";
		case GL_ERROR_INVALID_ENUM:
			return "GL_INVALID_ENUM";
		case GL_ERROR_INVALID_VALUE:
			return "GL_INVALID_VALUE";
		case GL_ERROR_INVALID_OPERATION:
			return "GL_INVALID_OPERATION";
		case GL_ERROR_STACK_OVERFLOW:
			return "GL_STACK_OVERFLOW";
		case GL_ERROR_STACK_UNDERFLOW:
			return "GL_STACK_UNDERFLOW";
		case GL_ERROR_OUT_OF_MEMORY:
			return "GL_OUT_OF_MEMORY";
		case GL_ERROR_INVALID_FRAMEBUFFER_OPERATION:
			return "GL_INVALID_FRAMEBUFFER_OPERATION";
		default:
			return "GL_UNKNOWN_ERROR";
		}
	}
}
