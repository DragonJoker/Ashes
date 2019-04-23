/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/Descriptor/DescriptorSetPool.hpp"

#include "AshesPP/Core/Device.hpp"
#include "AshesPP/Descriptor/DescriptorPool.hpp"
#include "AshesPP/Descriptor/DescriptorSet.hpp"
#include "AshesPP/Descriptor/DescriptorSetLayout.hpp"

namespace ashes
{
	namespace
	{
		VkDescriptorPoolSizeArray convert( VkDescriptorSetLayoutBindingArray const & bindings
			, uint32_t maxSets )
		{
			VkDescriptorPoolSizeArray result;

			for ( auto & binding : bindings )
			{
				result.push_back( { binding.descriptorType, binding.descriptorCount * maxSets } );
			}

			return result;
		}
	}

	DescriptorSetPool::DescriptorSetPool( Device const & device
		, DescriptorSetLayout const & layout
		, uint32_t maxSets
		, bool automaticFree )
		: m_layout{ layout }
		, m_maxSets{ maxSets }
		, m_pool{ device.createDescriptorPool( automaticFree ? VkDescriptorPoolCreateFlagBits::VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT : VkDescriptorPoolCreateFlagBits( 0u )
			, maxSets
			, convert( layout.getBindings(), maxSets ) ) }
	{
	}

	DescriptorSetPtr DescriptorSetPool::createDescriptorSet( uint32_t bindingPoint )const
	{
		return m_pool->createDescriptorSet( m_layout, bindingPoint );
	}
}
