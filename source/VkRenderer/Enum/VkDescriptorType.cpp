#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkDescriptorType convert( ashes::DescriptorType const & type )
	{
		switch ( type )
		{
		case ashes::DescriptorType::eSampler:
			return VK_DESCRIPTOR_TYPE_SAMPLER;

		case ashes::DescriptorType::eCombinedImageSampler:
			return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

		case ashes::DescriptorType::eSampledImage:
			return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;

		case ashes::DescriptorType::eStorageImage:
			return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;

		case ashes::DescriptorType::eUniformTexelBuffer:
			return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;

		case ashes::DescriptorType::eStorageTexelBuffer:
			return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;

		case ashes::DescriptorType::eUniformBuffer:
			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

		case ashes::DescriptorType::eStorageBuffer:
			return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

		case ashes::DescriptorType::eUniformBufferDynamic:
			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;

		case ashes::DescriptorType::eStorageBufferDynamic:
			return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;

		case ashes::DescriptorType::eInputAttachment:
			return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;

		default:
			assert( false );
			return VK_DESCRIPTOR_TYPE_SAMPLER;
		}
	}
}
