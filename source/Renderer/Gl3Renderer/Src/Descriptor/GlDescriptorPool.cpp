#include "Descriptor/GlDescriptorPool.hpp"

#include "Core/GlDevice.hpp"
#include "Descriptor/GlDescriptorSet.hpp"

namespace gl_renderer
{
	DescriptorPool::DescriptorPool( Device const & device
		, renderer::DescriptorPoolCreateFlags flags
		, uint32_t maxSets
		, renderer::DescriptorPoolSizeArray poolSizes )
		: renderer::DescriptorPool{ device, flags }
	{
	}

	renderer::DescriptorSetPtr DescriptorPool::createDescriptorSet( renderer::DescriptorSetLayout const & layout
		, uint32_t bindingPoint )const
	{
		return std::make_unique< DescriptorSet >( *this, layout, bindingPoint );
	}
}
