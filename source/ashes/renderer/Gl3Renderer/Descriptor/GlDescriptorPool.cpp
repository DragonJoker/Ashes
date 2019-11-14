#include "Descriptor/GlDescriptorPool.hpp"

#include "Core/GlDevice.hpp"
#include "Descriptor/GlDescriptorSet.hpp"

#include "ashesgl3_api.hpp"

namespace ashes::gl3
{
	DescriptorPool::DescriptorPool( VkDevice device
		, VkDescriptorPoolCreateInfo createInfo )
		: m_device{ device }
		, m_flags{ createInfo.flags }
		, m_maxSets{ createInfo.maxSets }
		, m_poolSizes{ makeVector( createInfo.pPoolSizes, createInfo.poolSizeCount ) }
	{
	}

	DescriptorPool::~DescriptorPool()
	{
		for ( auto & set : m_sets )
		{
			delete get( set );
		}
	}

	void DescriptorPool::registerSet( VkDescriptorSet set )
	{
		if ( checkFlag( m_flags, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT ) )
		{
			assert( m_maxSets > m_sets.size() );
			m_sets.push_back( set );
		}
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
