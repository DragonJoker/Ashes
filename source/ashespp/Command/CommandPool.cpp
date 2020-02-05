/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Command/CommandPool.hpp"

#include "ashespp/Command/CommandBuffer.hpp"
#include "ashespp/Core/Device.hpp"

namespace ashes
{
	CommandPool::CommandPool( Device const & device
		, VkCommandPoolCreateInfo createInfo )
		: m_device{ device }
	{
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateCommandPool( m_device
			, &createInfo
			, nullptr
			, &m_internal );
		checkError( res, "CommandPool creation" );
		registerObject( m_device, "CommandPool", this );
	}

	CommandPool::~CommandPool()
	{
		unregisterObject( m_device, this );
		m_device.vkDestroyCommandPool( m_device, m_internal, nullptr );
	}

	CommandBufferPtr CommandPool::createCommandBuffer( VkCommandBufferLevel level )const
	{
		return std::make_unique< CommandBuffer >( m_device
			, *this
			, level );
	}
}
