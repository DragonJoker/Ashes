#include "Descriptor/TestDescriptorSetLayout.hpp"

#include "Core/TestDevice.hpp"

namespace test_renderer
{
	DescriptorSetLayout::DescriptorSetLayout( Device const & device
		, renderer::DescriptorSetLayoutBindingArray && bindings )
		: renderer::DescriptorSetLayout{ device, std::move( bindings ) }
	{
	}

	DescriptorSetLayout::~DescriptorSetLayout()
	{
	}
}
