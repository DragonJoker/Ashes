#include "GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	std::string getName( GlBufferParameter value )
	{
		switch ( value )
		{
		case GL_BUFFER_PARAMETER_SIZE:
			return "GL_BUFFER_SIZE";

		default:
			assert( false && "Unsupported GlBufferParameter" );
			return "GlBufferParameter_UNKNOWN";
		}
	}
}
