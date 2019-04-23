/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/TestCommandPool.hpp"

#include "Command/TestCommandBuffer.hpp"
#include "Core/TestDevice.hpp"

namespace test_renderer
{
	CommandPool::CommandPool( Device const & device
		, uint32_t queueFamilyIndex
		, ashes::CommandPoolCreateFlags flags )
		: ashes::CommandPool{ device, queueFamilyIndex, flags }
		, m_device{ device }
	{
	}

	CommandPool::~CommandPool()
	{
	}

	ashes::CommandBufferPtr CommandPool::createCommandBuffer( bool primary )const
	{
		return std::make_unique< CommandBuffer >( m_device
			, *this
			, primary );
	}
}
