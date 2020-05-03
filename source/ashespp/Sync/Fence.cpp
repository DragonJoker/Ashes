/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Sync/Fence.hpp"

#include "ashespp/Core/Device.hpp"

namespace ashes
{
	Fence::Fence( Device const & device
		, VkFenceCreateFlags flags )
		: m_device{ device }
	{
		VkFenceCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			nullptr,
			flags
		};
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateFence( device
			, &createInfo
			, nullptr
			, &m_internal );
		checkError( res, "Fence creation" );
		registerObject( m_device, "Fence", this );
	}

	Fence::~Fence()
	{
		unregisterObject( m_device, this );
		m_device.vkDestroyFence( m_device
			, m_internal
			, nullptr );
	}

	WaitResult Fence::wait( uint64_t timeout )const
	{
		auto res = m_device.vkWaitForFences( m_device
			, 1
			, &m_internal
			, VK_TRUE
			, timeout );
		auto result = res == VK_SUCCESS
			? WaitResult::eSuccess
			: ( res == VK_TIMEOUT
				? WaitResult::eTimeOut
				: WaitResult::eError );
		onWaitEnd( *this, result );
		return result;
	}

	void Fence::reset()const
	{
		auto res = m_device.vkResetFences( m_device
			, 1
			, &m_internal );
		checkError( res, "Fence reset" );
	}
}
