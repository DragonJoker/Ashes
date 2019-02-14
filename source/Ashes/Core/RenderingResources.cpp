/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Core/RenderingResources.hpp"

#include "Ashes/Buffer/StagingBuffer.hpp"
#include "Ashes/Core/Device.hpp"

namespace ashes
{
	RenderingResources::RenderingResources( Device const & device )
		: m_device{ device }
		, m_commandBuffer{ m_device.getGraphicsCommandPool().createCommandBuffer() }
		, m_imageAvailableSemaphore{ m_device.createSemaphore() }
		, m_finishedRenderingSemaphore{ m_device.createSemaphore() }
		, m_fence{ m_device.createFence( FenceCreateFlag::eSignaled ) }
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
}
