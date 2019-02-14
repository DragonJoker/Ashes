#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlImageTiling value )
	{
		switch ( value )
		{
		case gl_renderer::GL_OPTIMAL_TILING_EXT:
			return "GL_OPTIMAL_TILING_EXT";

		case gl_renderer::GL_LINEAR_TILING_EXT:
			return "GL_LINEAR_TILING_EXT";

		default:
			assert( false && "Unsupported GlImageTiling" );
			return "GlImageTiling_UNKNOWN";
		}
	}

	GlImageTiling convert( ashes::ImageTiling const & value )
	{
		switch ( value )
		{
		case ashes::ImageTiling::eOptimal:
			return GL_OPTIMAL_TILING_EXT;

		case ashes::ImageTiling::eLinear:
			return GL_LINEAR_TILING_EXT;

		default:
			assert( false && "Unsupported image tiling" );
			return GL_OPTIMAL_TILING_EXT;
		}
	}
}
