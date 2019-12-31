#include "GlRendererPrerequisites.hpp"

namespace ashes::gl
{
	GLenum convert( VkSamplerMipmapMode const & mode )
	{
		switch ( mode )
		{
		case VK_SAMPLER_MIPMAP_MODE_NEAREST:
			return GL_FILTER_NEAREST_MIPMAP_NEAREST;

		case VK_SAMPLER_MIPMAP_MODE_LINEAR:
			return GL_FILTER_LINEAR_MIPMAP_LINEAR;

		default:
			assert( false );
			return GL_FILTER_NEAREST_MIPMAP_NEAREST;
		}
	}
}
