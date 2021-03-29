#include "Descriptor/GlDescriptorSetLayout.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	DescriptorSetLayout::DescriptorSetLayout( VkAllocationCallbacks const * allocInfo
		, VkDevice device
		, VkDescriptorSetLayoutCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
		, m_bindings{ makeVector( m_createInfo.pBindings, m_createInfo.bindingCount ) }
	{
		registerObject( m_device, *this );
	}

	DescriptorSetLayout::~DescriptorSetLayout()
	{
		unregisterObject( m_device, *this );
	}
}
