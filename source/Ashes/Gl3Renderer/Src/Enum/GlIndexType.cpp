#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlIndexType value )
	{
		switch ( value )
		{
		case gl_renderer::GL_INDEX_TYPE_UINT16:
			return "GL_UNSIGNED_SHORT";

		case gl_renderer::GL_INDEX_TYPE_UINT32:
			return "GL_UNSIGNED_INT";

		default:
			assert( false && "Unsupported GlIndexType" );
			return "GlIndexType_UNKNOWN";
		}
	}

	GlIndexType convert( ashes::IndexType const & type )
	{
		switch ( type )
		{
		case ashes::IndexType::eUInt16:
			return GL_INDEX_TYPE_UINT16;

		case ashes::IndexType::eUInt32:
			return GL_INDEX_TYPE_UINT32;

		default:
			assert( false && "Unsupported index type" );
			return GL_INDEX_TYPE_UINT32;
		}
	}
}
