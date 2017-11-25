#include "DescriptorType.hpp"

namespace renderer
{
	VkDescriptorType convert( DescriptorType const & type )
	{
		switch ( type )
		{
		case DescriptorType::eSampler:
			return VK_DESCRIPTOR_TYPE_SAMPLER;

		case DescriptorType::eCombinedImageSampler:
			return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

		case DescriptorType::eSampledImage:
			return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;

		case DescriptorType::eStorageImage:
			return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;

		case DescriptorType::eUniformTexelBuffer:
			return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;

		case DescriptorType::eStorageTexelBuffer:
			return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;

		case DescriptorType::eUniformBuffer:
			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

		case DescriptorType::eStorageBuffer:
			return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

		case DescriptorType::eUniformBufferDynamic:
			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;

		case DescriptorType::eStorageBufferDynamic:
			return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;

		case DescriptorType::eInputAttachment:
			return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;

		default:
			assert( false );
			return VK_DESCRIPTOR_TYPE_SAMPLER;
		}
	}
}
