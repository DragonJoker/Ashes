#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
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

	GlMemoryPropertyFlag convert( ashes::MemoryPropertyFlags const & flags )
	{
		uint32_t result{ GL_STREAM_DRAW };

		if ( checkFlag( flags, ashes::MemoryPropertyFlag::eDeviceLocal ) )
		{
			result = GL_STATIC_DRAW;
		}
		else if ( checkFlag( flags, ashes::MemoryPropertyFlag::eHostVisible ) )
		{
			result = GL_DYNAMIC_DRAW;
		}

		return GlMemoryPropertyFlag( result );
	}
}
