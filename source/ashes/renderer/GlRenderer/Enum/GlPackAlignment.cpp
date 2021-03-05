#include "GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	std::string getName( GlPackAlignment value )
	{
		switch ( value )
		{
		case GL_UNPACK_ALIGNMENT:
			return "GL_UNPACK_ALIGNMENT";

		case GL_PACK_ALIGNMENT:
			return "GL_PACK_ALIGNMENT";

		default:
			assert( false && "Unsupported GlPackAlignment" );
			return "GlPackAlignment_UNKNOWN";
		}
	}
}
