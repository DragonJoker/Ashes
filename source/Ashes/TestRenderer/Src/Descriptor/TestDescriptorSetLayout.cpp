#include "Descriptor/TestDescriptorSetLayout.hpp"

#include "Core/TestDevice.hpp"

namespace test_renderer
{
	DescriptorSetLayout::DescriptorSetLayout( Device const & device
		, ashes::DescriptorSetLayoutBindingArray && bindings )
		: ashes::DescriptorSetLayout{ device, std::move( bindings ) }
	{
	}

	DescriptorSetLayout::~DescriptorSetLayout()
	{
	}
}
