#include "GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	std::string getName( GlBaseType value )
	{
		switch ( value )
		{
		case GL_BYTE:
			return "GL_BYTE";

		case GL_UNSIGNED_BYTE:
			return "GL_UNSIGNED_BYTE";

		case GL_SHORT:
			return "GL_SHORT";

		case GL_UNSIGNED_SHORT:
			return "GL_UNSIGNED_SHORT";

		case GL_INT:
			return "GL_INT";

		case GL_UNSIGNED_INT:
			return "GL_UNSIGNED_INT";

		case GL_FLOAT:
			return "GL_FLOAT";

		default:
			assert( false && "Unsupported GlBaseType" );
			return "GlBaseType_UNKNOWN";
		}
	}
}
