#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	GLenum convert( ashes::MipmapMode const & mode )
	{
		switch ( mode )
		{
		case ashes::MipmapMode::eNearest:
			return GL_FILTER_NEAREST_MIPMAP_NEAREST;

		case ashes::MipmapMode::eLinear:
			return GL_FILTER_LINEAR_MIPMAP_LINEAR;

		default:
			assert( false );
			return GL_FILTER_NEAREST_MIPMAP_NEAREST;
		}
	}
}
