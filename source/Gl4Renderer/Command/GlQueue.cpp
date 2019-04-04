/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/GlQueue.hpp"

#include "Miscellaneous/GlCallLogger.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "Command/Commands/GlCommandBase.hpp"
#include "Core/GlDevice.hpp"
#include "Sync/GlFence.hpp"
#include "Sync/GlSemaphore.hpp"
#include "Core/GlSwapChain.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	Queue::Queue( VkDevice device
		, VkDeviceQueueCreateInfo createInfo
		, uint32_t index )
		: m_device{ device }
		, m_flags{ createInfo.flags }
		, m_queueFamilyIndex{ createInfo.queueFamilyIndex }
		, m_queuePriority{ createInfo.pQueuePriorities[index] }
		, m_index{ index }
	{
	}

	VkResult Queue::submit( VkSubmitInfoArray const & values
		, VkFence fence )const
	{
		for ( auto & value : values )
		{
			submit( value, fence );
		}

		return VK_SUCCESS;
	}

	VkResult Queue::present( VkPresentInfoKHR const & presentInfo )const
	{
		auto itIndex = presentInfo.pImageIndices;
		auto itResult = presentInfo.pResults;

		for ( auto itSwapchain = presentInfo.pSwapchains;
			itSwapchain != presentInfo.pSwapchains + presentInfo.swapchainCount;
			++itIndex, ++itResult, ++itSwapchain )
		{
			*itResult = get( *itSwapchain )->present( *itIndex );
		}
		return VK_SUCCESS;
	}

	VkResult Queue::waitIdle()const
	{
		auto context = ( ( Device * )m_device )->getContext();
		glLogCall( context
			, glFinish );
		return VK_SUCCESS;
	}

	void Queue::submit( VkSubmitInfo const & value
		, VkFence fence )const
	{
		auto context = ( ( Device * )m_device )->getContext();

		for ( auto it = value.pCommandBuffers; it != value.pCommandBuffers + value.commandBufferCount; ++it )
		{
			auto & commandBuffer = *it;
			auto & glCommandBuffer = *( ( CommandBuffer * )commandBuffer );
			glCommandBuffer.initialiseGeometryBuffers();

			for ( auto & command : glCommandBuffer.getCommands() )
			{
				command->apply( context );
			}

			glCommandBuffer.applyPostSubmitActions( context );
		}
	}
}
