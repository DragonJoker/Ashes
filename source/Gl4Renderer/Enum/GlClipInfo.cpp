#include "GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	std::string getName( GlClipOrigin value )
	{
		switch ( value )
		{
		case GL_LOWER_LEFT:
			return "GL_LOWER_LEFT";

		case GL_UPPER_LEFT:
			return "GL_UPPER_LEFT";

		default:
			assert( false && "Unsupported GlClipOrigin" );
			return "GlClipOrigin_UNKNOWN";
		}
	}

	std::string getName( GlClipDepth value )
	{
		switch ( value )
		{
		case GL_NEGATIVE_ONE_TO_ONE:
			return "GL_NEGATIVE_ONE_TO_ONE";

		case GL_ZERO_TO_ONE:
			return "GL_ZERO_TO_ONE";

		default:
			assert( false && "Unsupported GlClipDepth" );
			return "GlClipDepth_UNKNOWN";
		}
	}
}
