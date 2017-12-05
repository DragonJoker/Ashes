/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "VkFence.hpp"

#include "VkDevice.hpp"

namespace vk_renderer
{
	Fence::Fence( renderer::Device const & device
		, renderer::FenceCreateFlags flags )
		: renderer::Fence{ device, flags }
		, m_fence{ static_cast< Device const & >( device ).getDevice()
			, convert( flags ) }
	{
	}

	renderer::WaitResult Fence::wait( uint32_t timeout )
	{
		auto res = m_fence.wait( timeout );
		return res == VK_SUCCESS
			? renderer::WaitResult::eSuccess
			: ( res == VK_TIMEOUT
				? renderer::WaitResult::eTimeOut
				: renderer::WaitResult::eError );
	}

	void Fence::reset()
	{
		m_fence.reset();
	}
}
