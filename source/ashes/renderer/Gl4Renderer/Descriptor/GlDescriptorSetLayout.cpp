#include "Descriptor/GlDescriptorSetLayout.hpp"

namespace ashes::gl4
{
	DescriptorSetLayout::DescriptorSetLayout( VkDevice device
		, VkDescriptorSetLayoutCreateInfo createInfo )
		: m_flags{ createInfo.flags }
		, m_bindings{ makeVector( createInfo.pBindings, createInfo.bindingCount ) }
	{
	}
}
