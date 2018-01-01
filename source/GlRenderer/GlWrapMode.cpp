#include "GlWrapMode.hpp"

namespace gl_renderer
{
	GLenum convert( renderer::WrapMode const & mode )
	{
		switch ( mode )
		{
		case renderer::WrapMode::eRepeat:
			return GL_REPEAT;

		case renderer::WrapMode::eMirroredRepeat:
			return GL_MIRRORED_REPEAT;

		case renderer::WrapMode::eClampToEdge:
			return GL_CLAMP_TO_EDGE;

		case renderer::WrapMode::eClampToBorder:
			return GL_CLAMP_TO_BORDER;

		case renderer::WrapMode::eMirrorClampToEdge:
			return GL_MIRROR_CLAMP_TO_EDGE;

		default:
			assert( false );
			return GL_REPEAT;
		}
	}
}
