#include "Descriptor/GlDescriptorSetLayout.hpp"

namespace ashes::gl4
{
	DescriptorSetLayout::DescriptorSetLayout( VkDevice device
		, VkDescriptorSetLayoutCreateInfo createInfo )
		: m_createInfo{ std::move( createInfo ) }
		, m_bindings{ makeVector( m_createInfo.pBindings, m_createInfo.bindingCount ) }
	{
	}
}
