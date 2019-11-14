/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "util/RenderingResources.hpp"

#include <ashespp/Core/Device.hpp>
#include <ashespp/Core/SwapChain.hpp>
#include <ashespp/Command/Queue.hpp>

namespace utils
{
	RenderingResources::RenderingResources( ashes::Device const & device
		, ashes::SwapChain const & swapChain
		, ashes::CommandPool const & commandPool )
		: m_device{ device }
		, m_swapChain{ swapChain }
		, m_commandBuffer{ commandPool.createCommandBuffer() }
		, m_imageAvailableSemaphore{ m_device.createSemaphore() }
		, m_finishedRenderingSemaphore{ m_device.createSemaphore() }
		, m_fence{ m_device.createFence( VK_FENCE_CREATE_SIGNALED_BIT ) }
	{
	}

	bool RenderingResources::waitRecord( uint64_t timeout )
	{
		bool res = m_fence->wait( timeout ) == ashes::WaitResult::eSuccess;

		if ( res )
		{
			m_fence->reset();
		}

		return res;
	}

	VkResult RenderingResources::present( ashes::Queue const & queue )
	{
		return queue.present( m_swapChain
			, m_imageIndex
			, *m_finishedRenderingSemaphore );
	}
}
