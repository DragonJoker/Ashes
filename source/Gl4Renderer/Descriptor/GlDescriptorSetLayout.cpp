#include "Descriptor/GlDescriptorSetLayout.hpp"

namespace ashes
{
	inline VkDescriptorSetLayoutBinding deepCopy( VkDescriptorSetLayoutBinding const & rhs )
	{
		return rhs;
	}
}

namespace ashes::gl4
{
	DescriptorSetLayout::DescriptorSetLayout( VkDevice device
		, VkDescriptorSetLayoutCreateInfo createInfo )
		: m_flags{ createInfo.flags }
		, m_bindings{ makeVector( createInfo.pBindings, createInfo.bindingCount ) }
	{
	}
}
