/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "Fence.hpp"

#include "Device.hpp"

namespace renderer
{
	Fence::Fence( Device const & device
		, FenceCreateFlags flags )
		: m_fence{ device.getDevice(), convert( flags ) }
	{
	}

	WaitResult Fence::wait( uint32_t timeout )
	{
		auto res = m_fence.wait( timeout );
		return res == VK_SUCCESS
			? WaitResult::eSuccess
			: ( res == VK_TIMEOUT
				? WaitResult::eTimeOut
				: WaitResult::eError );
	}

	void Fence::reset()
	{
		m_fence.reset();
	}
}
