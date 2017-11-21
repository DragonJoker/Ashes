#include "RenderingResources.hpp"

#include "StagingBuffer.hpp"

#include <VkLib/CommandPool.hpp>
#include <VkLib/LogicalDevice.hpp>
#include <VkLib/Queue.hpp>

namespace renderer
{
	RenderingResources::RenderingResources( vk::LogicalDevice const & device
		, vk::CommandPool const & commandPool )
		: m_device{ device }
		, m_commandBuffer{ *this, commandPool }
		, m_imageAvailableSemaphore{ device.createSemaphore() }
		, m_finishedRenderingSemaphore{ device.createSemaphore() }
		, m_fence{ device, VK_FENCE_CREATE_SIGNALED_BIT }
		, m_stagingBuffer{ std::make_shared< StagingBuffer >( *this ) }
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
