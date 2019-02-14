#include "Descriptor/GlDescriptorSetLayout.hpp"

namespace gl_renderer
{
	DescriptorSetLayout::DescriptorSetLayout( ashes::Device const & device
		, ashes::DescriptorSetLayoutBindingArray && bindings )
		: ashes::DescriptorSetLayout{ device, std::move( bindings ) }
	{
	}
}
