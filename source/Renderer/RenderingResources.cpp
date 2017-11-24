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
		, m_commandBuffer{ m_device, m_device.getDevice().getGraphicsCommandPool() }
		, m_imageAvailableSemaphore{ m_device.getDevice().createSemaphore() }
		, m_finishedRenderingSemaphore{ m_device.getDevice().createSemaphore() }
		, m_fence{ m_device.getDevice(), VK_FENCE_CREATE_SIGNALED_BIT }
	{
	}

	bool RenderingResources::waitRecord( uint32_t timeout )
	{
		bool res = m_fence.wait( timeout ) == VK_SUCCESS;

		if ( res )
		{
			m_fence.reset();
		}

		return res;
	}
}
