/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkCommandPool.hpp"

#include "VkCommandBuffer.hpp"
#include "VkDevice.hpp"

namespace vk_renderer
{
	CommandPool::CommandPool( Device const & device
		, uint32_t queueFamilyIndex
		, renderer::CommandPoolCreateFlags flags )
		: renderer::CommandPool{ device, queueFamilyIndex, flags }
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
		auto res = CreateCommandPool( m_device
			, &createInfo
			, nullptr
			, &m_commandPool );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "CommandPool creation failed: " + getLastError() };
		}
	}

	CommandPool::~CommandPool()
	{
		DestroyCommandPool( m_device, m_commandPool, nullptr );
	}

	renderer::CommandBufferPtr CommandPool::createCommandBuffer( bool primary )const
	{
		return std::make_unique< CommandBuffer >( m_device
			, *this
			, primary );
	}
}
