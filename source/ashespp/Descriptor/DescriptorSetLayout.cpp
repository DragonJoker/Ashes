/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Descriptor/DescriptorSetLayout.hpp"

#include "ashespp/Core/Device.hpp"
#include "ashespp/Descriptor/DescriptorSetPool.hpp"

#include <algorithm>

namespace ashes
{
	DescriptorSetLayout::DescriptorSetLayout( Device const & device
		, VkDescriptorSetLayoutBindingArray bindings )
		: DescriptorSetLayout{ device, "DescriptorSetLayout", bindings }
	{
	}
	
	DescriptorSetLayout::DescriptorSetLayout( Device const & device
		, std::string const & debugName
		, VkDescriptorSetLayoutBindingArray bindings )
		: m_device{ device }
		, m_bindings{ std::move( bindings ) }
	{
		VkDescriptorSetLayoutCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,  // sType
			nullptr,                                              // pNext
			0,                                                    // flags
			static_cast< uint32_t >( m_bindings.size() ),         // bindingCount
			m_bindings.data()                                     // pBindings
		};
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateDescriptorSetLayout( m_device
			, &createInfo
			, nullptr
			, &m_internal );
		checkError( res, "DescriptorSetLayout creation" );
		registerObject( m_device, debugName, *this );
	}

	DescriptorSetLayout::~DescriptorSetLayout()
	{
		unregisterObject( m_device, *this );
		m_device.vkDestroyDescriptorSetLayout( m_device
			, m_internal
			, nullptr );
	}

	VkDescriptorSetLayoutBinding const & DescriptorSetLayout::getBinding( uint32_t bindingPoint
		, uint32_t index )const
	{
		auto it = std::find_if( m_bindings.begin()
			, m_bindings.end()
			, [bindingPoint, index]( VkDescriptorSetLayoutBinding const & lookup )
		{
			return lookup.binding == bindingPoint
				&& index < lookup.descriptorCount;
		} );

		if ( it == m_bindings.end() )
		{
			throw std::range_error{ "Descriptor set layout binding was not found." };
		}

		return *it;
	}

	DescriptorSetPoolPtr DescriptorSetLayout::createPool( uint32_t maxSets
		, bool automaticFree )const
	{
		return std::make_unique< DescriptorSetPool >( m_device
			, *this
			, maxSets
			, automaticFree );
	}

	DescriptorSetPoolPtr DescriptorSetLayout::createPool( std::string const & debugName
		, uint32_t maxSets
		, bool automaticFree )const
	{
		return std::make_unique< DescriptorSetPool >( m_device
			, debugName
			, *this
			, maxSets
			, automaticFree );
	}
}
