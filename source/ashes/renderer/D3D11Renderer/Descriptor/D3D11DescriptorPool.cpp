#include "Descriptor/D3D11DescriptorPool.hpp"

#include "Descriptor/D3D11DescriptorSet.hpp"
#include "Descriptor/D3D11DescriptorSetLayout.hpp"
#include "Core/D3D11Device.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
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
