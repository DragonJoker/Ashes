#include "Descriptor/D3D11DescriptorPool.hpp"

#include "Descriptor/D3D11DescriptorSet.hpp"
#include "Descriptor/D3D11DescriptorSetLayout.hpp"
#include "Core/D3D11Device.hpp"

namespace d3d11_renderer
{
	DescriptorPool::DescriptorPool( Device const & device
		, renderer::DescriptorPoolCreateFlags flags
		, uint32_t maxSets
		, renderer::DescriptorPoolSizeArray poolSizes )
		: renderer::DescriptorPool{ device, flags }
		, m_device{ device }
	{
	}

	DescriptorPool::~DescriptorPool()
	{
	}

	renderer::DescriptorSetPtr DescriptorPool::createDescriptorSet( renderer::DescriptorSetLayout const & layout
		, uint32_t bindingPoint )const
	{
		return std::make_unique< DescriptorSet >( m_device
			, *this
			, static_cast< DescriptorSetLayout const & >( layout )
			, bindingPoint );
	}
}
