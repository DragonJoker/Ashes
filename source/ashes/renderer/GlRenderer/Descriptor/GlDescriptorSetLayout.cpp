#include "Descriptor/GlDescriptorSetLayout.hpp"

namespace ashes::gl
{
	DescriptorSetLayout::DescriptorSetLayout( VkDevice device
		, VkDescriptorSetLayoutCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
		, m_bindings{ makeVector( m_createInfo.pBindings, m_createInfo.bindingCount ) }
	{
	}
}
