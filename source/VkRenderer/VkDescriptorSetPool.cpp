#include "VkDescriptorSetPool.hpp"

#include "VkDescriptorSet.hpp"
#include "VkDescriptorSetLayout.hpp"

namespace vk_renderer
{
	DescriptorSetPool::DescriptorSetPool( renderer::DescriptorSetLayout const & layout
		, uint32_t maxSets )
		: renderer::DescriptorSetPool{ layout, maxSets }
		, m_pool{ static_cast< DescriptorSetLayout const & >( layout ).getLayout().createDescriptorPool( maxSets ) }
	{
	}

	renderer::DescriptorSetPtr DescriptorSetPool::createDescriptorSet()const
	{
		return std::make_unique< DescriptorSet >( *this );
	}
}
