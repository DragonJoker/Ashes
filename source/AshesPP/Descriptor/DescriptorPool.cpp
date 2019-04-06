/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/Descriptor/DescriptorPool.hpp"

#include "AshesPP/Core/Device.hpp"
#include "AshesPP/Descriptor/DescriptorSet.hpp"

namespace ashes
{
	DescriptorPool::DescriptorPool( Device const & device
		, VkDescriptorPoolCreateFlags flags
		, uint32_t maxSets
		, VkDescriptorPoolSizeArray poolSizes )
		: m_device{ device }
		, m_automaticFree{ ( flags & ( ~VkDescriptorPoolCreateFlagBits::VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT ) ) == 0 }
	{
		std::vector< VkDescriptorPoolSize > vkpoolSizes;

		for ( auto const & poolSize : poolSizes )
		{
			vkpoolSizes.push_back(
				{
					poolSize.type,
					poolSize.descriptorCount
				} );
		}

		VkDescriptorPoolCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,      // sType
			nullptr,                                            // pNext
			flags,                                              // flags
			maxSets,                                            // maxSets
			uint32_t( vkpoolSizes.size() ),                     // poolSizeCount
			vkpoolSizes.data()                                  // pPoolSizes
		};
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateDescriptorPool( m_device
			, &createInfo
			, nullptr
			, &m_internal );
		checkError( res, "DescriptorPool creation" );
		registerObject( m_device, "DescriptorPool", this );
	}

	DescriptorPool::~DescriptorPool()
	{
		unregisterObject( m_device, this );
		m_device.vkDestroyDescriptorPool( m_device, m_internal, nullptr );
	}

	DescriptorSetPtr DescriptorPool::createDescriptorSet( DescriptorSetLayout const & layout
		, uint32_t bindingPoint )const
	{
		return std::make_unique< DescriptorSet >( m_device
			, *this
			, layout
			, bindingPoint );
	}
}
