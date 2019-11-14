#include "GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	std::string getName( GlShaderInfo value )
	{
		switch ( value )
		{
		case GL_INFO_COMPILE_STATUS:
			return "GL_COMPILE_STATUS";

		case GL_INFO_VALIDATE_STATUS:
			return "GL_VALIDATE_STATUS";

		case GL_INFO_LINK_STATUS:
			return "GL_LINK_STATUS";

		case GL_INFO_LOG_LENGTH:
			return "GL_INFO_LOG_LENGTH";

		case GL_INFO_ATTACHED_SHADERS:
			return "GL_ATTACHED_SHADERS";

		default:
			assert( false && "Unsupported GlShaderInfo" );
			return "GlShaderInfo_UNKNOWN";
		}
	}
}
