#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	GLuint convert( renderer::MemoryMapFlags const & flags )
	{
		GLuint result{ 0 };

		if ( checkFlag( flags, renderer::MemoryMapFlag::eRead ) )
		{
			result = GL_MAP_READ_BIT;
		}
		else if ( checkFlag( flags, renderer::MemoryMapFlag::eWrite ) )
		{
			result = GL_MAP_WRITE_BIT;
		}
		else if ( checkFlag( flags, renderer::MemoryMapFlag::ePersistent ) )
		{
			result = GL_MAP_PERSISTENT_BIT;
		}
		else if ( checkFlag( flags, renderer::MemoryMapFlag::eCoherent ) )
		{
			result = GL_MAP_COHERENT_BIT;
		}

		if ( checkFlag( flags, renderer::MemoryMapFlag::eInvalidateRange ) )
		{
			result |= GL_MAP_INVALIDATE_RANGE_BIT;
		}

		if ( checkFlag( flags, renderer::MemoryMapFlag::eInvalidateBuffer ) )
		{
			result |= GL_MAP_INVALIDATE_BUFFER_BIT;
		}

		if ( checkFlag( flags, renderer::MemoryMapFlag::eFlushExplicit ) )
		{
			result |= GL_MAP_FLUSH_EXPLICIT_BIT;
		}

		if ( checkFlag( flags, renderer::MemoryMapFlag::eUnsynchronised ) )
		{
			result |= GL_MAP_UNSYNCHRONIZED_BIT;
		}

		return result;
	}
}
