/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Sync/VkFence.hpp"

#include "Core/VkDevice.hpp"

namespace vk_renderer
{
	Fence::Fence( Device const & device
		, ashes::FenceCreateFlags flags )
		: ashes::Fence{ device, flags }
		, m_device{ device }
	{
		VkFenceCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			nullptr,
			convert( flags )   // flags
		};
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateFence( device
			, &createInfo
			, nullptr
			, &m_fence );
		checkError( res, "Fence creation" );
	}

	Fence::~Fence()
	{
		m_device.vkDestroyFence( m_device
			, m_fence
			, nullptr );
	}

	ashes::WaitResult Fence::wait( uint64_t timeout )const
	{
		auto res = m_device.vkWaitForFences( m_device
			, 1
			, &m_fence
			, VK_TRUE
			, timeout );
		return res == VK_SUCCESS
			? ashes::WaitResult::eSuccess
			: ( res == VK_TIMEOUT
				? ashes::WaitResult::eTimeOut
				: ashes::WaitResult::eError );
	}

	void Fence::reset()const
	{
		auto res = m_device.vkResetFences( m_device
			, 1
			, &m_fence );
		checkError( res, "Fence reset" );
	}
}
