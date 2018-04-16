/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Descriptor/DescriptorPool.hpp"

#include "Core/Device.hpp"

namespace renderer
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
