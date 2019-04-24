#include "Descriptor/D3D11DescriptorPool.hpp"

#include "Descriptor/D3D11DescriptorSet.hpp"
#include "Descriptor/D3D11DescriptorSetLayout.hpp"
#include "Core/D3D11Device.hpp"

namespace ashes::d3d11
{
	DescriptorPool::DescriptorPool( VkDevice device
		, VkDescriptorPoolCreateFlags flags
		, uint32_t maxSets
		, ashes::VkDescriptorPoolSizeArray poolSizes )
		: ashes::DescriptorPool{ device, flags }
		, m_device{ device }
	{
	}

	DescriptorPool::~DescriptorPool()
	{
	}

	ashes::DescriptorSetPtr DescriptorPool::createDescriptorSet( ashes::DescriptorSetLayout const & layout
		, uint32_t bindingPoint )const
	{
		return std::make_unique< DescriptorSet >( m_device
			, *this
			, static_cast< DescriptorSetLayout const & >( layout )
			, bindingPoint );
	}
}
