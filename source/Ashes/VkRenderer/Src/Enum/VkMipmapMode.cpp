#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkSamplerMipmapMode convert( ashes::MipmapMode const & mode )
	{
		switch ( mode )
		{
		case ashes::MipmapMode::eNone:
			return VK_SAMPLER_MIPMAP_MODE_NEAREST;

		case ashes::MipmapMode::eNearest:
			return VK_SAMPLER_MIPMAP_MODE_NEAREST;

		case ashes::MipmapMode::eLinear:
			return VK_SAMPLER_MIPMAP_MODE_LINEAR;

		default:
			assert( false );
			return VK_SAMPLER_MIPMAP_MODE_NEAREST;
		}
	}
}
