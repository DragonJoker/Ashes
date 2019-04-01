#include "Descriptor/GlDescriptorPool.hpp"

#include "Core/GlDevice.hpp"
#include "Descriptor/GlDescriptorSet.hpp"

namespace gl_renderer
{
	DescriptorPool::DescriptorPool( Device const & device
		, VkDescriptorPoolCreateFlags flags
		, uint32_t maxSets
		, ashes::DescriptorPoolSizeArray poolSizes )
		: ashes::DescriptorPool{ device, flags }
	{
	}

	ashes::DescriptorSetPtr DescriptorPool::createDescriptorSet( ashes::DescriptorSetLayout const & layout
		, uint32_t bindingPoint )const
	{
		return std::make_unique< DescriptorSet >( *this, layout, bindingPoint );
	}
}
