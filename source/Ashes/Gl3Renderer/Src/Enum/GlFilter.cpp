#include "GlRendererPrerequisites.hpp"

namespace gl_renderer
{
	std::string getName( GlFilter value )
	{
		switch ( value )
		{
		case gl_renderer::GL_FILTER_NEAREST:
			return "GL_NEAREST";

		case gl_renderer::GL_FILTER_LINEAR:
			return "GL_LINEAR";

		case gl_renderer::GL_FILTER_NEAREST_MIPMAP_NEAREST:
			return "GL_NEAREST_MIPMAP_NEAREST";

		case gl_renderer::GL_FILTER_LINEAR_MIPMAP_NEAREST:
			return "GL_LINEAR_MIPMAP_NEAREST";

		case gl_renderer::GL_FILTER_NEAREST_MIPMAP_LINEAR:
			return "GL_NEAREST_MIPMAP_LINEAR";

		case gl_renderer::GL_FILTER_LINEAR_MIPMAP_LINEAR:
			return "GL_LINEAR_MIPMAP_LINEAR";

		default:
			assert( false && "Unsupported GlFilter" );
			return "GLFilter_UNKNOWN";
		}
	}

	GlFilter convert( ashes::Filter const & filter )
	{
		switch ( filter )
		{
		case ashes::Filter::eNearest:
			return GL_FILTER_NEAREST;

		case ashes::Filter::eLinear:
			return GL_FILTER_LINEAR;

		default:
			assert( false );
			return GL_FILTER_NEAREST;
		}
	}

	GlFilter convert( ashes::Filter const & filter
		, ashes::MipmapMode mode
		, float minLod
		, float maxLod )
	{
		switch ( filter )
		{
		case ashes::Filter::eNearest:
			switch ( mode )
			{
			case ashes::MipmapMode::eNone:
				return GL_FILTER_NEAREST;

			case ashes::MipmapMode::eNearest:
				return GL_FILTER_NEAREST_MIPMAP_NEAREST;

			case ashes::MipmapMode::eLinear:
				return GL_FILTER_NEAREST_MIPMAP_LINEAR;

			default:
				assert( false );
				return GL_FILTER_NEAREST;
			}

		case ashes::Filter::eLinear:
			switch ( mode )
			{
			case ashes::MipmapMode::eNone:
				return GL_FILTER_LINEAR;

			case ashes::MipmapMode::eNearest:
				return GL_FILTER_LINEAR_MIPMAP_NEAREST;

			case ashes::MipmapMode::eLinear:
				return GL_FILTER_LINEAR_MIPMAP_LINEAR;

			default:
				assert( false );
				return GL_FILTER_LINEAR;
			}

		default:
			assert( false );
			return GL_FILTER_NEAREST;
		}
	}
}
