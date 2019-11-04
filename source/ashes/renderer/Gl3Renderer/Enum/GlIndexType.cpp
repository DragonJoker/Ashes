#include "GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	std::string getName( GlIndexType value )
	{
		switch ( value )
		{
		case GL_INDEX_TYPE_UINT16:
			return "GL_UNSIGNED_SHORT";

		case GL_INDEX_TYPE_UINT32:
			return "GL_UNSIGNED_INT";

		default:
			assert( false && "Unsupported GlIndexType" );
			return "GlIndexType_UNKNOWN";
		}
	}

	GlIndexType convert( VkIndexType const & type )
	{
		switch ( type )
		{
		case VK_INDEX_TYPE_UINT16:
			return GL_INDEX_TYPE_UINT16;

		case VK_INDEX_TYPE_UINT32:
			return GL_INDEX_TYPE_UINT32;

		default:
			assert( false && "Unsupported index type" );
			return GL_INDEX_TYPE_UINT32;
		}
	}
}
