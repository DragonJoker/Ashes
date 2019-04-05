#include "Descriptor/GlDescriptorPool.hpp"

#include "Core/GlDevice.hpp"
#include "Descriptor/GlDescriptorSet.hpp"

namespace ashes
{
	inline VkDescriptorPoolSize deepCopy( VkDescriptorPoolSize const & rhs )
	{
		return rhs;
	}
}

namespace ashes::gl4
{
	DescriptorPool::DescriptorPool( VkDevice device
		, VkDescriptorPoolCreateInfo createInfo )
		: m_flags{ createInfo.flags }
		, m_maxSets{ createInfo.maxSets }
		, m_poolSizes{ makeVector( createInfo.pPoolSizes, createInfo.poolSizeCount ) }
	{
	}

	VkResult DescriptorPool::reset( VkDescriptorPoolResetFlags flags )
	{
		return VK_SUCCESS;
	}

	VkResult DescriptorPool::free( VkDescriptorSetArray sets )
	{
		return VK_SUCCESS;
	}
}
