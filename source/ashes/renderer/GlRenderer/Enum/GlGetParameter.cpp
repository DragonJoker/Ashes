#include "GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	std::string getName( GlGetParameter value )
	{
		switch ( value )
		{
		case GL_SMOOTH_LINE_WIDTH_RANGE:
			return "GL_SMOOTH_LINE_WIDTH_RANGE";
			
		case GL_ALIASED_LINE_WIDTH_RANGE:
			return "GL_ALIASED_LINE_WIDTH_RANGE";
			
		case GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT:
			return "GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT";

		default:
			assert( false && "Unsupported GlGetParameter" );
			return "GlGetParameter_UNKNOWN";
		}
	}
}
