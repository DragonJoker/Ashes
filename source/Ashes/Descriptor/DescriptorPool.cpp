/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Descriptor/DescriptorPool.hpp"

#include "Ashes/Core/Device.hpp"

namespace ashes
{
	DescriptorPool::DescriptorPool( Device const & device
		, DescriptorPoolCreateFlags flags )
		: m_device{ device }
		, m_automaticFree{ !checkFlag( flags, DescriptorPoolCreateFlag::eFreeDescriptorSet ) }
	{
		registerObject( m_device, "DescriptorPool", this );
	}

	DescriptorPool::~DescriptorPool()
	{
		unregisterObject( m_device, this );
	}
}
