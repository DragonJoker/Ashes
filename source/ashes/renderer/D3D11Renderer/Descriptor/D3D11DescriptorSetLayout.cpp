#include "Descriptor/D3D11DescriptorSetLayout.hpp"

#include "Core/D3D11Device.hpp"

namespace ashes::d3d11
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
			throw std::range_error{ "Descriptor set layout binding was not found." };
		}

		return *it;
	}
}
