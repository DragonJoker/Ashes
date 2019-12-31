#include "GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	namespace gl3
	{
		std::string getName( GlBufferMemoryPropertyFlag value )
		{
			switch ( value )
			{
			case GL_STREAM_DRAW:
				return "GL_STREAM_DRAW";
			case GL_STREAM_READ:
				return "GL_STREAM_READ";
			case GL_STREAM_COPY:
				return "GL_STREAM_COPY";
			case GL_STATIC_DRAW:
				return "GL_STATIC_DRAW";
			case GL_STATIC_READ:
				return "GL_STATIC_READ";
			case GL_STATIC_COPY:
				return "GL_STATIC_COPY";
			case GL_DYNAMIC_DRAW:
				return "GL_DYNAMIC_DRAW";
			case GL_DYNAMIC_READ:
				return "GL_DYNAMIC_READ";
			case GL_DYNAMIC_COPY:
				return "GL_DYNAMIC_COPY";
			}

			return "Unsupported buffer memory flag.";
		}

		GlBufferMemoryPropertyFlag getBufferMemoryPropertyFlag( VkMemoryPropertyFlags const & flags )
		{
			GlBufferMemoryPropertyFlag result{ GL_STREAM_DRAW };

			if ( checkFlag( flags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT ) )
			{
				result = GL_STATIC_DRAW;
			}
			else if ( checkFlag( flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) )
			{
				result = GL_DYNAMIC_DRAW;
			}

			return result;
		}
	}

	namespace gl4
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

				if ( ashes::checkFlag( flags, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ) )
				{
					result |= GL_MEMORY_PROPERTY_PERSISTENT_BIT;
					result |= GL_MEMORY_PROPERTY_COHERENT_BIT;
				}
			}

			return result;
		}
	}
}
