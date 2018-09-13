#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlMemoryMapFlags value )
	{
		std::string result;
		std::string sep;

		if ( checkFlag( value, gl_renderer::GlMemoryMapFlag::GL_MEMORY_MAP_READ_BIT ) )
		{
			result += sep + "GL_MAP_READ_BIT";
			sep = " | ";
		}

		if ( checkFlag( value, gl_renderer::GlMemoryMapFlag::GL_MEMORY_MAP_WRITE_BIT ) )
		{
			result += sep + "GL_MAP_WRITE_BIT";
			sep = " | ";
		}

		if ( checkFlag( value, gl_renderer::GlMemoryMapFlag::GL_MEMORY_MAP_PERSISTENT_BIT ) )
		{
			result += sep + "GL_MAP_PERSISTENT_BIT";
			sep = " | ";
		}

		if ( checkFlag( value, gl_renderer::GlMemoryMapFlag::GL_MEMORY_MAP_COHERENT_BIT ) )
		{
			result += sep + "GL_MAP_COHERENT_BIT";
			sep = " | ";
		}

		if ( checkFlag( value, gl_renderer::GlMemoryMapFlag::GL_MEMORY_MAP_INVALIDATE_RANGE_BIT ) )
		{
			result += sep + "GL_MAP_INVALIDATE_RANGE_BIT";
			sep = " | ";
		}

		if ( checkFlag( value, gl_renderer::GlMemoryMapFlag::GL_MEMORY_MAP_INVALIDATE_BUFFER_BIT ) )
		{
			result += sep + "GL_MAP_INVALIDATE_BUFFER_BIT";
			sep = " | ";
		}

		if ( checkFlag( value, gl_renderer::GlMemoryMapFlag::GL_MEMORY_MAP_FLUSH_EXPLICIT_BIT ) )
		{
			result += sep + "GL_MAP_FLUSH_EXPLICIT_BIT";
			sep = " | ";
		}

		if ( checkFlag( value, gl_renderer::GlMemoryMapFlag::GL_MEMORY_MAP_UNSYNCHRONIZED_BIT ) )
		{
			result += sep + "GL_MAP_UNSYNCHRONIZED_BIT";
		}

		return result;
	}

	GlMemoryMapFlags convert( ashes::MemoryMapFlags const & flags )
	{
		GlMemoryMapFlags result{ 0 };

		if ( checkFlag( flags, ashes::MemoryMapFlag::eRead ) )
		{
			result |= GL_MEMORY_MAP_READ_BIT;
		}

		if ( checkFlag( flags, ashes::MemoryMapFlag::eWrite ) )
		{
			result |= GL_MEMORY_MAP_WRITE_BIT;
		}

		if ( checkFlag( flags, ashes::MemoryMapFlag::ePersistent ) )
		{
			result |= GL_MEMORY_MAP_PERSISTENT_BIT;
		}

		if ( checkFlag( flags, ashes::MemoryMapFlag::eCoherent ) )
		{
			result |= GL_MEMORY_MAP_COHERENT_BIT;
		}

		if ( checkFlag( flags, ashes::MemoryMapFlag::eInvalidateRange ) )
		{
			result |= GL_MEMORY_MAP_INVALIDATE_RANGE_BIT;
		}

		if ( checkFlag( flags, ashes::MemoryMapFlag::eInvalidateBuffer ) )
		{
			result |= GL_MEMORY_MAP_INVALIDATE_BUFFER_BIT;
		}

		if ( checkFlag( flags, ashes::MemoryMapFlag::eUnsynchronised ) )
		{
			result |= GL_MEMORY_MAP_UNSYNCHRONIZED_BIT;
		}

		result |= GL_MEMORY_MAP_FLUSH_EXPLICIT_BIT;
		return result;
	}
}
