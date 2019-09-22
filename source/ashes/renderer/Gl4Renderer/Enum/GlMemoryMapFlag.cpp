#include "GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	std::string getName( GlMemoryMapFlags value )
	{
		std::string result;
		std::string sep;

		if ( ashes::checkFlag( value, GlMemoryMapFlag::GL_MEMORY_MAP_READ_BIT ) )
		{
			result += sep + "GL_MAP_READ_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryMapFlag::GL_MEMORY_MAP_WRITE_BIT ) )
		{
			result += sep + "GL_MAP_WRITE_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryMapFlag::GL_MEMORY_MAP_PERSISTENT_BIT ) )
		{
			result += sep + "GL_MAP_PERSISTENT_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryMapFlag::GL_MEMORY_MAP_COHERENT_BIT ) )
		{
			result += sep + "GL_MAP_COHERENT_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryMapFlag::GL_MEMORY_MAP_INVALIDATE_RANGE_BIT ) )
		{
			result += sep + "GL_MAP_INVALIDATE_RANGE_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryMapFlag::GL_MEMORY_MAP_INVALIDATE_BUFFER_BIT ) )
		{
			result += sep + "GL_MAP_INVALIDATE_BUFFER_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryMapFlag::GL_MEMORY_MAP_FLUSH_EXPLICIT_BIT ) )
		{
			result += sep + "GL_MAP_FLUSH_EXPLICIT_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryMapFlag::GL_MEMORY_MAP_UNSYNCHRONIZED_BIT ) )
		{
			result += sep + "GL_MAP_UNSYNCHRONIZED_BIT";
		}

		return result;
	}

	GlMemoryMapFlags convertMemoryMapFlags( VkMemoryPropertyFlags flags )
	{
		GlMemoryMapFlags result{ 0 };

		if ( ashes::checkFlag( flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) )
		{
			result |= GL_MEMORY_MAP_READ_BIT;
			result |= GL_MEMORY_MAP_WRITE_BIT;
			result |= GL_MEMORY_MAP_INVALIDATE_RANGE_BIT;

#if AshesGL4_UsePersistentMapping

			result |= GL_MEMORY_MAP_PERSISTENT_BIT;

			if ( ashes::checkFlag( flags, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ) )
			{
				result |= GL_MEMORY_MAP_COHERENT_BIT;
			}
			else
			{
				result |= GL_MEMORY_MAP_FLUSH_EXPLICIT_BIT;
			}

#endif
		}

			return result;
	}
}
