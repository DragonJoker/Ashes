#include "Descriptor/TestDescriptorPool.hpp"

#include "Descriptor/TestDescriptorSet.hpp"
#include "Descriptor/TestDescriptorSetLayout.hpp"
#include "Core/TestDevice.hpp"

namespace test_renderer
{
	DescriptorPool::DescriptorPool( Device const & device
		, ashes::DescriptorPoolCreateFlags flags
		, uint32_t maxSets
		, ashes::DescriptorPoolSizeArray poolSizes )
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
