#include "GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	std::string getName( GlMemoryPropertyFlags value )
	{
		std::string result;
		std::string sep;

		if ( ashes::checkFlag( value, GlMemoryPropertyFlag::GL_MEMORY_PROPERTY_READ_BIT ) )
		{
			result += sep + "GL_MAP_READ_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryPropertyFlag::GL_MEMORY_PROPERTY_WRITE_BIT ) )
		{
			result += sep + "GL_MAP_WRITE_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryPropertyFlag::GL_MEMORY_PROPERTY_DYNAMIC_STORAGE_BIT ) )
		{
			result += sep + "GL_DYNAMIC_STORAGE_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryPropertyFlag::GL_MEMORY_PROPERTY_COHERENT_BIT ) )
		{
			result += sep + "GL_MAP_COHERENT_BIT";
			sep = " | ";
		}

		if ( ashes::checkFlag( value, GlMemoryPropertyFlag::GL_MEMORY_PROPERTY_PERSISTENT_BIT ) )
		{
			result += sep + "GL_MAP_PERSISTENT_BIT";
		}

		return result;
	}

	GlMemoryPropertyFlags convertMemoryPropertyFlags( VkMemoryPropertyFlags const & flags )
	{
		GlMemoryPropertyFlags result{ 0 };

		if ( ashes::checkFlag( flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) )
		{
			result |= GL_MEMORY_PROPERTY_READ_BIT;
			result |= GL_MEMORY_PROPERTY_WRITE_BIT;
			//result |= GL_MEMORY_PROPERTY_PERSISTENT_BIT;

			//if ( ashes::checkFlag( flags, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ) )
			//{
			//	result |= GL_MEMORY_PROPERTY_COHERENT_BIT;
			//}
		}

		return result;
	}
}
