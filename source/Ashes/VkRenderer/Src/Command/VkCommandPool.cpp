/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/VkCommandPool.hpp"

#include "Command/VkCommandBuffer.hpp"
#include "Core/VkDevice.hpp"

namespace vk_renderer
{
	CommandPool::CommandPool( Device const & device
		, uint32_t queueFamilyIndex
		, ashes::CommandPoolCreateFlags flags )
		: ashes::CommandPool{ device, queueFamilyIndex, flags }
		, m_device{ device }
	{
		VkCommandPoolCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			nullptr,
			convert( flags ),                         // flags
			queueFamilyIndex                          // queueFamilyIndex
		};
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateCommandPool( m_device
			, &createInfo
			, nullptr
			, &m_commandPool );
		checkError( res, "CommandPool creation" );
	}

	CommandPool::~CommandPool()
	{
		m_device.vkDestroyCommandPool( m_device, m_commandPool, nullptr );
	}

	ashes::CommandBufferPtr CommandPool::createCommandBuffer( bool primary )const
	{
		return std::make_unique< CommandBuffer >( m_device
			, *this
			, primary );
	}
}
