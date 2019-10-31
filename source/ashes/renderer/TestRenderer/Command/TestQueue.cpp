/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/TestQueue.hpp"

#include "Command/Commands/TestCommandBase.hpp"
#include "Command/TestCommandBuffer.hpp"
#include "Core/TestDevice.hpp"
#include "Core/TestSwapChain.hpp"
#include "Sync/TestFence.hpp"
#include "Sync/TestSemaphore.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	Queue::Queue( VkDevice device
		, VkDeviceQueueCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
	}

	VkResult Queue::submit( VkSubmitInfoArray const & infos
		, VkFence fence )const
	{
		for ( auto & info : infos )
		{
			doSubmit( { info.pCommandBuffers, info.pCommandBuffers + info.commandBufferCount }
				, { info.pWaitSemaphores, info.pWaitSemaphores + info.waitSemaphoreCount }
				, { info.pWaitDstStageMask, info.pWaitDstStageMask + info.waitSemaphoreCount }
				, { info.pSignalSemaphores, info.pSignalSemaphores + info.signalSemaphoreCount }
				, fence );
		}

		return VK_SUCCESS;
	}

	VkResult Queue::present( VkPresentInfoKHR const & presentInfo )const
	{
		auto itIndices = presentInfo.pImageIndices;

		if ( presentInfo.pResults )
		{
			auto itResult = presentInfo.pResults;

			for ( auto & swapChain : makeArrayView( presentInfo.pSwapchains, presentInfo.swapchainCount ) )
			{
				*itResult = get( swapChain )->present( *itIndices );
				++itResult;
				++itIndices;
			}
		}
		else
		{
			for ( auto & swapChain : makeArrayView( presentInfo.pSwapchains, presentInfo.swapchainCount ) )
			{
				get( swapChain )->present( *itIndices );
				++itIndices;
			}
		}

		return VK_SUCCESS;
	}

	VkResult Queue::waitIdle()const
	{
		return VK_SUCCESS;
	}

#if VK_EXT_debug_utils

	void Queue::beginDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const
	{
		m_label = DebugLabel
		{
			{ labelInfo.color[0], labelInfo.color[1], labelInfo.color[2], labelInfo.color[3] },
			labelInfo.pLabelName,
		};
	}

	void Queue::endDebugUtilsLabel()const
	{
		m_label = std::nullopt;
	}

	void Queue::insertDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const
	{
		m_label = DebugLabel
		{
			{ labelInfo.color[0], labelInfo.color[1], labelInfo.color[2], labelInfo.color[3] },
			labelInfo.pLabelName,
		};
	}

#endif
#if VK_EXT_debug_marker

	void Queue::debugMarkerBegin( VkDebugMarkerMarkerInfoEXT const & labelInfo )const
	{
		m_label = DebugLabel
		{
			{ labelInfo.color[0], labelInfo.color[1], labelInfo.color[2], labelInfo.color[3] },
			labelInfo.pMarkerName,
		};
	}

	void Queue::debugMarkerEnd()const
	{
		m_label = std::nullopt;
	}

	void Queue::debugMarkerInsert( VkDebugMarkerMarkerInfoEXT const & labelInfo )const
	{
		m_label = DebugLabel
		{
			{ labelInfo.color[0], labelInfo.color[1], labelInfo.color[2], labelInfo.color[3] },
			labelInfo.pMarkerName,
		};
	}

#endif

	VkResult Queue::doSubmit( VkCommandBufferArray const & commandBuffers
		, VkSemaphoreArray const & semaphoresToWait
		, VkPipelineStageFlagsArray const & semaphoresStage
		, VkSemaphoreArray const & semaphoresToSignal
		, VkFence fence )const
	{
		for ( auto & commandBuffer : commandBuffers )
		{
			auto & dxCommandBuffer = *get( commandBuffer );
			dxCommandBuffer.execute();
		}

		return VK_SUCCESS;
	}
}
