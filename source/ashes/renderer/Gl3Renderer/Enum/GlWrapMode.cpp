#include "GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	std::string getName( GlWrapMode value )
	{
		switch ( value )
		{
		case GL_WRAP_MODE_REPEAT:
			return "GL_REPEAT";

		case GL_WRAP_MODE_MIRRORED_REPEAT:
			return "GL_MIRRORED_REPEAT";

		case GL_WRAP_MODE_CLAMP_TO_EDGE:
			return "GL_CLAMP_TO_EDGE";

		case GL_WRAP_MODE_CLAMP_TO_BORDER:
			return "GL_CLAMP_TO_BORDER";

		case GL_WRAP_MODE_MIRROR_CLAMP_TO_EDGE:
			return "GL_CLAMP_TO_EDGE";

		default:
			assert( false && "Unsupported GlWrapMode" );
			return "GlWrapMode_UNKNOWN";
		}
	}

	GlWrapMode convert( VkSamplerAddressMode const & mode )
	{
		switch ( mode )
		{
		case VK_SAMPLER_ADDRESS_MODE_REPEAT:
			return GL_WRAP_MODE_REPEAT;

		case VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT:
			return GL_WRAP_MODE_MIRRORED_REPEAT;

		case VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE:
			return GL_WRAP_MODE_CLAMP_TO_EDGE;

		case VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER:
			return GL_WRAP_MODE_CLAMP_TO_BORDER;

		case VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE:
			return GL_WRAP_MODE_MIRROR_CLAMP_TO_EDGE;

		default:
			assert( false );
			return GL_WRAP_MODE_REPEAT;
		}
	}
}
