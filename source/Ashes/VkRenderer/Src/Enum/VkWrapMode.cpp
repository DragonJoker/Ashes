#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkSamplerAddressMode convert( ashes::WrapMode const & mode )
	{
		switch ( mode )
		{
		case ashes::WrapMode::eRepeat:
			return VK_SAMPLER_ADDRESS_MODE_REPEAT;

		case ashes::WrapMode::eMirroredRepeat:
			return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;

		case ashes::WrapMode::eClampToEdge:
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

		case ashes::WrapMode::eClampToBorder:
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;

		case ashes::WrapMode::eMirrorClampToEdge:
			return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;

		default:
			assert( false );
			return VK_SAMPLER_ADDRESS_MODE_REPEAT;
		}
	}
}
