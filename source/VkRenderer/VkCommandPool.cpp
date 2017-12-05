/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkCommandPool.hpp"

#include "VkCommandBuffer.hpp"
#include "VkDevice.hpp"

namespace vk_renderer
{
	CommandPool::CommandPool( renderer::Device const & device
		, uint32_t queueFamilyIndex
		, renderer::CommandPoolCreateFlags flags )
		: renderer::CommandPool{ device, queueFamilyIndex, flags }
		, m_ownedCommandPool{ std::make_unique< vk::CommandPool >( static_cast< Device const & >( device ).getDevice()
			, queueFamilyIndex
			, convert( flags ) ) }
		, m_nonOwnedCommandPool{ m_ownedCommandPool.get() }
	{
	}

	CommandPool::CommandPool( renderer::Device const & device
		, vk::CommandPool const & pool )
		: renderer::CommandPool{ device, 0u, 0u }
		, m_nonOwnedCommandPool{ &pool }
	{
	}

	renderer::CommandBufferPtr CommandPool::createCommandBuffer( bool primary )const
	{
		return std::make_unique< CommandBuffer >( m_device
			, *this
			, primary );
	}
}
