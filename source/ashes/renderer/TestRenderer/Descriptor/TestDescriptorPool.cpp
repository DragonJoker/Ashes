#include "Descriptor/TestDescriptorPool.hpp"

#include "Descriptor/TestDescriptorSet.hpp"
#include "Descriptor/TestDescriptorSetLayout.hpp"
#include "Core/TestDevice.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	DescriptorPool::DescriptorPool( VkDevice device
		, VkDescriptorPoolCreateInfo createInfos )
		: m_device{ device }
		, m_poolSizes{ makeVector( createInfos.pPoolSizes, createInfos.poolSizeCount ) }
		, m_createInfos{ std::move( createInfos ) }
	{
		m_createInfos.pPoolSizes = m_poolSizes.data();
	}

	DescriptorPool::~DescriptorPool()
	{
		for ( auto & set : m_sets )
		{
			deallocate( set, nullptr );
		}
	}

	void DescriptorPool::registerSet( VkDescriptorSet set )
	{
		if ( checkFlag( m_createInfos.flags, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT ) )
		{
			m_sets.push_back( set );
		}

		m_allSets.push_back( set );
	}

	VkResult DescriptorPool::reset( VkDescriptorPoolResetFlags flags )
	{
		for ( auto & set : m_allSets )
		{
			deallocate( set, nullptr );
		}

		m_allSets.clear();
		m_sets.clear();
		return VK_SUCCESS;
	}

	VkResult DescriptorPool::free( VkDescriptorSetArray sets )
	{
		if ( checkFlag( m_createInfos.flags, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT ) )
		{
			for ( auto set : sets )
			{
				auto it = std::find( m_sets.begin()
					, m_sets.end()
					, set );

				if ( it != m_sets.end() )
				{
					deallocate( *it, nullptr );
					m_sets.erase( it );
				}
			}
		}

		return VK_SUCCESS;
	}
}
