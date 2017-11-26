/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "CommandPool.hpp"

#include "CommandBuffer.hpp"

namespace renderer
{
	CommandPool::CommandPool( Device const & device
		, uint32_t queueFamilyIndex
		, CommandPoolCreateFlags flags )
		: m_device{ device }
		, m_ownedCommandPool{ std::make_unique< vk::CommandPool >( device.getDevice()
			, queueFamilyIndex
			, convert( flags ) ) }
		, m_nonOwnedCommandPool{ m_ownedCommandPool.get() }
	{
	}

	CommandPool::CommandPool( Device const & device
		, vk::CommandPool const & pool )
		: m_device{ device }
		, m_nonOwnedCommandPool{ &pool }
	{
	}

	CommandBufferPtr CommandPool::createCommandBuffer()const
	{
		return std::make_unique< CommandBuffer >( m_device, *this );
	}
}
