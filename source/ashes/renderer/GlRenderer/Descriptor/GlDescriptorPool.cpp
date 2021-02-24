#include "Descriptor/GlDescriptorPool.hpp"

#include "Core/GlDevice.hpp"
#include "Descriptor/GlDescriptorSet.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	DescriptorPool::DescriptorPool( VkAllocationCallbacks const * allocInfo
		, VkDevice device
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
			auto ires = m_sets.insert( set );
			assert( ires.second );
		}
	}

	VkResult DescriptorPool::reset( VkDescriptorPoolResetFlags flags )
	{
		m_sets.clear();
		return VK_SUCCESS;
	}

	VkResult DescriptorPool::free( VkDescriptorSetArray sets )
	{
		for ( auto & set : sets )
		{
			auto it = m_sets.find( set );

			if ( it != m_sets.end() )
			{
				m_sets.erase( it );
			}
		}

		return VK_SUCCESS;
	}
}
