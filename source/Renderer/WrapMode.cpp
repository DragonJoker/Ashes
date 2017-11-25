#include "WrapMode.hpp"

namespace renderer
{
	VkSamplerAddressMode convert( WrapMode const & mode )
	{
		switch ( mode )
		{
		case WrapMode::eRepeat:
			return VK_SAMPLER_ADDRESS_MODE_REPEAT;

		case WrapMode::eMirroredRepeat:
			return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;

		case WrapMode::eClampToEdge:
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

		case WrapMode::eClampToBorder:
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;

		case WrapMode::eMirrorClampToEdge:
			return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;

		default:
			assert( false );
			return VK_SAMPLER_ADDRESS_MODE_REPEAT;
		}
	}
}
