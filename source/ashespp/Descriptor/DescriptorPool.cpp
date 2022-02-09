/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Descriptor/DescriptorPool.hpp"

#include "ashespp/Core/Device.hpp"
#include "ashespp/Descriptor/DescriptorSet.hpp"

namespace ashes
{
	DescriptorPool::DescriptorPool( Device const & device
		, VkDescriptorPoolCreateFlags flags
		, uint32_t maxSets
		, VkDescriptorPoolSizeArray poolSizes )
		: DescriptorPool{ device, "DescriptorPool", flags, maxSets, poolSizes }
	{
	}
	
	DescriptorPool::DescriptorPool( Device const & device
		, std::string const & debugName
		, VkDescriptorPoolCreateFlags flags
		, uint32_t maxSets
		, VkDescriptorPoolSizeArray poolSizes )
		: VkObject{ debugName }
		, m_device{ device }
		, m_automaticFree{ !checkFlag( flags, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT ) }
	{
		VkDescriptorPoolCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,      // sType
			nullptr,                                            // pNext
			flags,                                              // flags
			maxSets,                                            // maxSets
			uint32_t( poolSizes.size() ),                       // poolSizeCount
			poolSizes.data()                                    // pPoolSizes
		};
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateDescriptorPool( m_device
			, &createInfo
			, m_device.getAllocationCallbacks()
			, &m_internal );
		checkError( res, "DescriptorPool creation" );
		registerObject( m_device, debugName, *this );
	}

	DescriptorPool::~DescriptorPool()
	{
		unregisterObject( m_device, *this );
		m_device.vkDestroyDescriptorPool( m_device
			, m_internal
			, m_device.getAllocationCallbacks() );
	}

	DescriptorSetPtr DescriptorPool::createDescriptorSet( DescriptorSetLayout const & layout
		, uint32_t bindingPoint
		, void * pNext )const
	{
		return std::make_unique< DescriptorSet >( m_device
			, *this
			, layout
			, bindingPoint
			, pNext );
	}
	
	DescriptorSetPtr DescriptorPool::createDescriptorSet( std::string const & debugName
		, DescriptorSetLayout const & layout
		, uint32_t bindingPoint
		, void * pNext )const
	{
		return std::make_unique< DescriptorSet >( m_device
			, debugName
			, *this
			, layout
			, bindingPoint
			, pNext );
	}

	void DescriptorPool::freeDescriptorSet( DescriptorSetPtr set )const
	{
		assert( ( !m_automaticFree ) && "freeDescriptorSet can only be used if VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT is set" );
		auto res = m_device.vkFreeDescriptorSets( m_device, m_internal, 1u, &static_cast< VkDescriptorSet const & >( *set ) );
		checkError( res, "DescriptorPool::freeDescriptorSet" );
	}
}
