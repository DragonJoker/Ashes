#include "GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	std::string getName( GlFilter value )
	{
		switch ( value )
		{
		case GL_FILTER_NEAREST:
			return "GL_NEAREST";

		case GL_FILTER_LINEAR:
			return "GL_LINEAR";

		case GL_FILTER_NEAREST_MIPMAP_NEAREST:
			return "GL_NEAREST_MIPMAP_NEAREST";

		case GL_FILTER_LINEAR_MIPMAP_NEAREST:
			return "GL_LINEAR_MIPMAP_NEAREST";

		case GL_FILTER_NEAREST_MIPMAP_LINEAR:
			return "GL_NEAREST_MIPMAP_LINEAR";

		case GL_FILTER_LINEAR_MIPMAP_LINEAR:
			return "GL_LINEAR_MIPMAP_LINEAR";

		default:
			assert( false && "Unsupported GlFilter" );
			return "GLFilter_UNKNOWN";
		}
	}

	GlFilter convert( VkFilter const & filter )
	{
		switch ( filter )
		{
		case VK_FILTER_NEAREST:
			return GL_FILTER_NEAREST;

		case VK_FILTER_LINEAR:
			return GL_FILTER_LINEAR;

		default:
			assert( false );
			return GL_FILTER_NEAREST;
		}
	}

	GlFilter convert( VkFilter const & filter
		, VkSamplerMipmapMode mode
		, float minLod
		, float maxLod )
	{
		switch ( filter )
		{
		case VK_FILTER_NEAREST:
			switch ( mode )
			{
			case VK_SAMPLER_MIPMAP_MODE_NEAREST:
				return GL_FILTER_NEAREST_MIPMAP_NEAREST;

			case VK_SAMPLER_MIPMAP_MODE_LINEAR:
				return GL_FILTER_NEAREST_MIPMAP_LINEAR;

			default:
				assert( false );
				return GL_FILTER_NEAREST;
			}

		case VK_FILTER_LINEAR:
			switch ( mode )
			{
			case VK_SAMPLER_MIPMAP_MODE_NEAREST:
				return GL_FILTER_LINEAR_MIPMAP_NEAREST;

			case VK_SAMPLER_MIPMAP_MODE_LINEAR:
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
