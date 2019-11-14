#include "Descriptor/TestDescriptorSetLayout.hpp"

#include "Core/TestDevice.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	DescriptorSetLayout::DescriptorSetLayout( VkDevice device
		, VkDescriptorSetLayoutCreateInfo createInfo )
		: m_device{ device }
		, m_bindings{ createInfo.pBindings, createInfo.pBindings + createInfo.bindingCount }
		, m_createInfo{ std::move( createInfo ) }
	{
		m_createInfo.pBindings = m_bindings.data();
	}

	DescriptorSetLayout::~DescriptorSetLayout()
	{
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
			get( m_device )->onReportMessage( VK_DEBUG_REPORT_ERROR_BIT_EXT
				, VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT
				, uint64_t( get( this ) )
				, 0u
				, VK_ERROR_INCOMPATIBLE_DRIVER
				, "Direct3D11"
				, "Descriptor set layout binding was not found." );
			static VkDescriptorSetLayoutBinding dummy;
			return dummy;
		}

		return *it;
	}
}
