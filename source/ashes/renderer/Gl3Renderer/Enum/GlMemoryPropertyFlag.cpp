#include "GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	std::string getName( GlMemoryPropertyFlag value )
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

	GlMemoryPropertyFlag convert( VkMemoryPropertyFlags const & flags )
	{
		GlMemoryPropertyFlag result{ GL_STREAM_DRAW };

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
