/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesRenderer/Command/CommandPool.hpp"

#include "AshesRenderer/Command/CommandBuffer.hpp"
#include "AshesRenderer/Core/Device.hpp"

namespace ashes
{
	CommandPool::CommandPool( Device const & device
		, uint32_t queueFamilyIndex
		, CommandPoolCreateFlags flags )
		: m_device{ device }
	{
		registerObject( m_device, "CommandPool", this );
	}

	CommandPool::~CommandPool()
	{
		unregisterObject( m_device, this );
	}
}
