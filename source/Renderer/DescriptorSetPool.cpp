#include "DescriptorSetPool.hpp"

#include "DescriptorSet.hpp"
#include "DescriptorSetLayout.hpp"

namespace renderer
{
	DescriptorSetPool::DescriptorSetPool( DescriptorSetLayout const & layout )
		: m_pool{ layout.getLayout().createDescriptorPool() }
	{
	}

	DescriptorSet DescriptorSetPool::createDescriptorSet()const
	{
		return DescriptorSet{ *this };
	}
}
