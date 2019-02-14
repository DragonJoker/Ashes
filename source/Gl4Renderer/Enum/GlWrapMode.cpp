#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlWrapMode value )
	{
		switch ( value )
		{
		case gl_renderer::GL_WRAP_MODE_REPEAT:
			return "GL_REPEAT";

		case gl_renderer::GL_WRAP_MODE_MIRRORED_REPEAT:
			return "GL_MIRRORED_REPEAT";

		case gl_renderer::GL_WRAP_MODE_CLAMP_TO_EDGE:
			return "GL_CLAMP_TO_EDGE";

		case gl_renderer::GL_WRAP_MODE_CLAMP_TO_BORDER:
			return "GL_CLAMP_TO_BORDER";

		case gl_renderer::GL_WRAP_MODE_MIRROR_CLAMP_TO_EDGE:
			return "GL_CLAMP_TO_EDGE";

		default:
			assert( false && "Unsupported GlWrapMode" );
			return "GlWrapMode_UNKNOWN";
		}
	}

	GlWrapMode convert( ashes::WrapMode const & mode )
	{
		switch ( mode )
		{
		case ashes::WrapMode::eRepeat:
			return GL_WRAP_MODE_REPEAT;

		case ashes::WrapMode::eMirroredRepeat:
			return GL_WRAP_MODE_MIRRORED_REPEAT;

		case ashes::WrapMode::eClampToEdge:
			return GL_WRAP_MODE_CLAMP_TO_EDGE;

		case ashes::WrapMode::eClampToBorder:
			return GL_WRAP_MODE_CLAMP_TO_BORDER;

		case ashes::WrapMode::eMirrorClampToEdge:
			return GL_WRAP_MODE_MIRROR_CLAMP_TO_EDGE;

		default:
			assert( false );
			return GL_WRAP_MODE_REPEAT;
		}
	}
}
