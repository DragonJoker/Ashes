/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkDescriptorSetLayoutBinding.hpp"

namespace vk_renderer
{
	vk::DescriptorLayoutBinding convert( renderer::DescriptorSetLayoutBinding const & binding )
	{
		return vk::DescriptorLayoutBinding
		{
			binding.getBindingPoint(),
			convert( binding.getDescriptorType() ),
			1u,
			convert( binding.getShaderStageFlags() ),
		};
	}
}
