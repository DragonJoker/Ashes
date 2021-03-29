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
		m_memory.resize( m_maxSets * sizeof( DescriptorSet ) );
		DescriptorSet * buffer = reinterpret_cast< DescriptorSet * >( m_memory.data() );

		for ( uint32_t i = 0u; i < m_maxSets; ++i )
		{
			m_free.push_back( get( buffer ) );
			++buffer;
		}

		registerObject( m_device, *this );
	}

	DescriptorPool::~DescriptorPool()
	{
		unregisterObject( m_device, *this );

		for ( auto & set : m_sets )
		{
			get( set )->~DescriptorSet();
		}
	}

	VkResult DescriptorPool::allocate( VkDescriptorSetLayout layout
		, VkDescriptorSet & set )
	{
		if ( m_free.empty() )
		{
			return VK_ERROR_OUT_OF_POOL_MEMORY;
		}

		VkResult result = VK_ERROR_INITIALIZATION_FAILED;

		try
		{
			set = m_free.back();
			m_free.erase( m_free.begin() + ( m_free.size() - 1 ) );
			new ( set )DescriptorSet{ nullptr, get( this ), layout };
			auto ires = m_sets.insert( set );
			assert( ires.second );
		}
		catch ( Exception & exc )
		{
			result = exc.getResult();
			reportError( get( this ), result, "Allocation", exc.what() );
		}
		catch ( std::exception & exc )
		{
			reportError( get( this ), result, "Allocation", exc.what() );
		}
		catch ( ... )
		{
			reportError( get( this ), result, "Allocation", "Unknown error" );
		}

		return result;
	}

	VkResult DescriptorPool::reset( VkDescriptorPoolResetFlags flags )
	{
		for ( auto set : m_sets )
		{
			get( set )->~DescriptorSet();
			m_free.push_back( set );
		}

		m_sets.clear();
		return VK_SUCCESS;
	}

	VkResult DescriptorPool::free( ArrayView< VkDescriptorSet const > const & sets )
	{
		if ( checkFlag( m_flags, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT ) )
		{
			for ( auto set : sets )
			{
				auto it = m_sets.find( set );

				if ( it != m_sets.end() )
				{
					get( set )->~DescriptorSet();
					m_free.push_back( set );
					m_sets.erase( it );
				}
			}
		}

		return VK_SUCCESS;
	}
}
