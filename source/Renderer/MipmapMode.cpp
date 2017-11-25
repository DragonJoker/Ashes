#include "MipmapMode.hpp"

namespace renderer
{
	VkSamplerMipmapMode convert( MipmapMode const & mode )
	{
		switch ( mode )
		{
		case MipmapMode::eNearest:
			return VK_SAMPLER_MIPMAP_MODE_NEAREST;

		case MipmapMode::eLinear:
			return VK_SAMPLER_MIPMAP_MODE_LINEAR;

		default:
			assert( false );
			return VK_SAMPLER_MIPMAP_MODE_NEAREST;
		}
	}
}
