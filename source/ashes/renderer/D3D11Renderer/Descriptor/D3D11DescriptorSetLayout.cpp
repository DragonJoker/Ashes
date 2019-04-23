#include "Descriptor/D3D11DescriptorSetLayout.hpp"

#include "Core/D3D11Device.hpp"

namespace ashes::d3d11
{
	DescriptorSetLayout::DescriptorSetLayout( Device const & device
		, VkDescriptorSetLayoutBindingArray bindings )
		: ashes::DescriptorSetLayout{ device, std::move( bindings ) }
	{
	}

	DescriptorSetLayout::~DescriptorSetLayout()
	{
	}
}
