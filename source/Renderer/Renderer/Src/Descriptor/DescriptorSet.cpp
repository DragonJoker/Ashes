/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Descriptor/DescriptorSet.hpp"

#include "Descriptor/DescriptorSetPool.hpp"

namespace renderer
{
	DescriptorSet::DescriptorSet( DescriptorPool const & pool, uint32_t bindingPoint )
		: m_bindingPoint{ bindingPoint }
		, m_pool{ pool }
	{
	}

	DescriptorSet::~DescriptorSet()
	{
	}
}
