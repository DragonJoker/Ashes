#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	GLenum convert( renderer::IndexType const & type )
	{
		switch ( type )
		{
		case renderer::IndexType::eUInt16:
			return GL_UNSIGNED_SHORT;

		case renderer::IndexType::eUInt32:
			return GL_UNSIGNED_INT;

		default:
			assert( false && "Unsupported index type" );
			return GL_UNSIGNED_INT;
		}
	}
}
