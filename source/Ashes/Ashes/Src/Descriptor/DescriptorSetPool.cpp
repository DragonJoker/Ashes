/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Descriptor/DescriptorSetPool.hpp"

#include "Core/Device.hpp"
#include "Descriptor/DescriptorPool.hpp"
#include "Descriptor/DescriptorSet.hpp"
#include "Descriptor/DescriptorSetLayout.hpp"

namespace ashes
{
	namespace
	{
		DescriptorPoolSizeArray convert( DescriptorSetLayoutBindingArray const & bindings
			, uint32_t maxSets )
		{
			DescriptorPoolSizeArray result;

			for ( auto & binding : bindings )
			{
				result.push_back( { binding.getDescriptorType(), binding.getDescriptorsCount() * maxSets } );
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
		, m_pool{ device.createDescriptorPool( automaticFree ? ashes::DescriptorPoolCreateFlag::eFreeDescriptorSet : ashes::DescriptorPoolCreateFlag( 0u )
			, maxSets
			, convert( layout.getBindings(), maxSets ) ) }
	{
	}

	DescriptorSetPtr DescriptorSetPool::createDescriptorSet( uint32_t bindingPoint )const
	{
		return m_pool->createDescriptorSet( m_layout, bindingPoint );
	}
}
