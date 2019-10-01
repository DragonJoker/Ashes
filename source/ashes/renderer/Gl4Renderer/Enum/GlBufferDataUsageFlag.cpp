#include "GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	std::string getName( GlBufferDataUsageFlags value )
	{
		switch ( value )
		{
		case GL_BUFFER_DATA_USAGE_STREAM_DRAW:
			return "GL_STREAM_DRAW";
		case GL_BUFFER_DATA_USAGE_STREAM_READ:
			return "GL_STREAM_READ";
		case GL_BUFFER_DATA_USAGE_STREAM_COPY:
			return "GL_STREAM_COPY";
		case GL_BUFFER_DATA_USAGE_STATIC_DRAW:
			return "GL_STATIC_DRAW";
		case GL_BUFFER_DATA_USAGE_STATIC_READ:
			return "GL_STATIC_READ";
		case GL_BUFFER_DATA_USAGE_STATIC_COPY:
			return "GL_STATIC_COPY";
		case GL_BUFFER_DATA_USAGE_DYNAMIC_DRAW:
			return "GL_DYNAMIC_DRAW";
		case GL_BUFFER_DATA_USAGE_DYNAMIC_READ:
			return "GL_DYNAMIC_READ";
		case GL_BUFFER_DATA_USAGE_DYNAMIC_COPY:
			return "GL_DYNAMIC_COPY";
		default:
			return "Unknown";
		}
	}

	GlBufferDataUsageFlags getBufferDataUsageFlags( VkMemoryPropertyFlags const & flags )
	{
		auto result{ GlBufferDataUsageFlags( 0 ) };

		if ( ashes::checkFlag( flags, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) )
		{
			if ( ashes::checkFlag( flags, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT ) )
			{
				result = GL_BUFFER_DATA_USAGE_STREAM_COPY;
			}
			else
			{
				result = GL_BUFFER_DATA_USAGE_DYNAMIC_COPY;
			}
		}
		else
		{
			result = GL_BUFFER_DATA_USAGE_STATIC_COPY;
		}

		return result;
	}
}
