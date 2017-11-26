#include "RenderingResources.hpp"

#include "Device.hpp"
#include "StagingBuffer.hpp"

#include <VkLib/CommandPool.hpp>
#include <VkLib/LogicalDevice.hpp>
#include <VkLib/Queue.hpp>

namespace renderer
{
	RenderingResources::RenderingResources( Device const & device )
		: m_device{ device }
		, m_commandBuffer{ m_device, m_device.getGraphicsCommandPool() }
		, m_imageAvailableSemaphore{ std::make_unique< Semaphore >( m_device ) }
		, m_finishedRenderingSemaphore{ std::make_unique< Semaphore >( m_device ) }
		, m_fence{ m_device, FenceCreateFlag::eSignaled }
	{
	}

	bool RenderingResources::waitRecord( uint32_t timeout )
	{
		bool res = m_fence.wait( timeout ) == WaitResult::eSuccess;

		if ( res )
		{
			m_fence.reset();
		}

		return res;
	}
}
