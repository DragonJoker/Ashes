#include "GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	std::string getName( GlAccessType value )
	{
		switch ( value )
		{
		case GL_ACCESS_TYPE_READ_ONLY:
			return "GL_ACCESS_TYPE_READ_ONLY";

		case GL_ACCESS_TYPE_WRITE_ONLY:
			return "GL_ACCESS_TYPE_WRITE_ONLY";

		case GL_ACCESS_TYPE_READ_WRITE:
			return "GL_ACCESS_TYPE_READ_WRITE";

		default:
			assert( false && "Unsupported GlAccessType" );
			return "GlAccessType_UNKNOWN";
		}
	}

	GLenum convertAccessFlags( VkAccessFlags const & flags )
	{
		GLenum result{ 0 };
		return result;
	}

	VkAccessFlags convertAccessFlags( GLenum flags )
	{
		VkAccessFlags result{ 0u };
		return result;
	}
}
