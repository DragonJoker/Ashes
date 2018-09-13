/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Descriptor/DescriptorPool.hpp"

#include "Core/Device.hpp"

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
