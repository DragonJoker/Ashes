#include "GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	std::string getName( GlImageTiling value )
	{
		switch ( value )
		{
		case GL_OPTIMAL_TILING_EXT:
			return "GL_OPTIMAL_TILING_EXT";

		case GL_LINEAR_TILING_EXT:
			return "GL_LINEAR_TILING_EXT";

		default:
			assert( false && "Unsupported GlImageTiling" );
			return "GlImageTiling_UNKNOWN";
		}
	}

	GlImageTiling convert( VkImageTiling const & value )
	{
		switch ( value )
		{
		case VK_IMAGE_TILING_OPTIMAL:
			return GL_OPTIMAL_TILING_EXT;

		case VK_IMAGE_TILING_LINEAR:
			return GL_LINEAR_TILING_EXT;

		default:
			assert( false && "Unsupported image tiling" );
			return GL_OPTIMAL_TILING_EXT;
		}
	}
}
