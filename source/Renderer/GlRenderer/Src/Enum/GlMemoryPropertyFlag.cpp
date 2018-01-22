#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	GLbitfield convert( renderer::MemoryPropertyFlags const & flags )
	{
		GLuint result{ 0 };

		if ( checkFlag( flags, renderer::MemoryPropertyFlag::eHostVisible ) )
		{
			result = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT;
		}

		if ( checkFlag( flags, renderer::MemoryPropertyFlag::eHostCoherent ) )
		{
			result = GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT;
		}

		return result;
	}
}
