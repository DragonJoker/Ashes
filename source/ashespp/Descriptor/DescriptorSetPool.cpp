/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Descriptor/DescriptorSetPool.hpp"

#include "ashespp/Core/Device.hpp"
#include "ashespp/Descriptor/DescriptorPool.hpp"
#include "ashespp/Descriptor/DescriptorSet.hpp"
#include "ashespp/Descriptor/DescriptorSetLayout.hpp"

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