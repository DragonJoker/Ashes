/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkDescriptorSetLayoutBinding convert( ashes::DescriptorSetLayoutBinding const & binding )
	{
		return VkDescriptorSetLayoutBinding
		{
			binding.getBindingPoint(),
			convert( binding.getDescriptorType() ),
			binding.getDescriptorsCount(),
			convert( binding.getShaderStageFlags() ),
		};
	}
}
