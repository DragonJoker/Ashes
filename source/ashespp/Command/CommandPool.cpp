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
		, uint32_t queueFamilyIndex
		, VkCommandPoolCreateFlags flags )
		: m_device{ device }
	{
		VkCommandPoolCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			nullptr,
			flags,                                    // flags
			queueFamilyIndex                          // queueFamilyIndex
		};
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

	CommandBufferPtr CommandPool::createCommandBuffer( bool primary )const
	{
		return std::make_unique< CommandBuffer >( m_device
			, *this
			, primary );
	}
}
