#include "GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	std::string getName( GlGetParameter value )
	{
		switch ( value )
		{
		case GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT:
			return "GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT";

		default:
			assert( false && "Unsupported GlGetParameter" );
			return "GlGetParameter_UNKNOWN";
		}
	}
}
