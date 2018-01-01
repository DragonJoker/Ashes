#include "GlCullModeFlag.hpp"

namespace gl_renderer
{
	GLenum convert( renderer::CullModeFlags const & flags )
	{
		GLenum result{ 0 };

		if ( checkFlag( flags, renderer::CullModeFlag::eFrontAndBack ) )
		{
			result = GL_FRONT_AND_BACK;
		}
		else if ( checkFlag( flags, renderer::CullModeFlag::eFront ) )
		{
			result = GL_FRONT;
		}
		else if ( checkFlag( flags, renderer::CullModeFlag::eBack ) )
		{
			result = GL_BACK;
		}

		return result;
	}
}
