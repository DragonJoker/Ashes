/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Command/Queue.hpp"

#include "ashespp/Command/CommandBuffer.hpp"
#include "ashespp/Core/Device.hpp"
#include "ashespp/Core/SwapChain.hpp"
#include "ashespp/Sync/Fence.hpp"
#include "ashespp/Sync/Semaphore.hpp"

namespace ashes
{
	Queue::Queue( Device const & device
		, uint32_t familyIndex
		, uint32_t index )
		: m_device{ device }
		, m_familyIndex{ familyIndex }
		, m_index{ index }
	{
		m_device.vkGetDeviceQueue( m_device
			, m_familyIndex
			, m_index
			, &m_internal );
		registerObject( m_device, "Queue", this );
	}

	Queue::~Queue()
	{
		unregisterObject( m_device, this );
	}

	VkResult Queue::present( SwapChain const & swapChain
		, uint32_t imageIndex
		, Semaphore const & semaphoreToWait )const
	{
		UInt32Array indices;
		indices.push_back( imageIndex );
		return present( SwapChainCRefArray{ { std::ref( swapChain ) } }
			, indices
			, SemaphoreCRefArray{ { std::ref( semaphoreToWait ) } } ).front();
	}

	void Queue::submit( CommandBufferCRefArray const & commandBuffers
		, SemaphoreCRefArray const & semaphoresToWait
		, VkPipelineStageFlagsArray const & semaphoresStage
		, SemaphoreCRefArray const & semaphoresToSignal
		, Fence const * fence )const
	{
		auto vkCommandBuffers = makeVkArray< VkCommandBuffer >( commandBuffers );
		auto vkSemaphoresToWait = makeVkArray< VkSemaphore >( semaphoresToWait );
		auto vkSemaphoresToSignal = makeVkArray< VkSemaphore >( semaphoresToSignal );

		std::vector< VkSubmitInfo > submitInfo
		{
			{
				VK_STRUCTURE_TYPE_SUBMIT_INFO,
				nullptr,
				static_cast< uint32_t >( vkSemaphoresToWait.size() ),      // waitSemaphoreCount
				vkSemaphoresToWait.empty()                                 // pWaitSemaphores
					? nullptr
					: vkSemaphoresToWait.data(),
				semaphoresStage.empty()                                    // pWaitDstStageMask
					? nullptr
					: semaphoresStage.data(),
				static_cast< uint32_t >( vkCommandBuffers.size() ),        // commandBufferCount
				vkCommandBuffers.empty()                                   // pCommandBuffers
					? nullptr
					: vkCommandBuffers.data(),
				static_cast< uint32_t >( vkSemaphoresToSignal.size() ),    // signalSemaphoreCount
				vkSemaphoresToSignal.empty()                               // pSignalSemaphores
					? nullptr
					: vkSemaphoresToSignal.data()
			}
		};
		DEBUG_DUMP( submitInfo );
		auto res = m_device.vkQueueSubmit( m_internal
			, static_cast< uint32_t >( submitInfo.size() )
			, submitInfo.data()
			, fence ? static_cast< VkFence >( *fence ) : VK_NULL_HANDLE );
		checkError( res, "Queue submit" );
	}

	VkResultArray Queue::present( SwapChainCRefArray const & swapChains
		, UInt32Array const & imagesIndex
		, SemaphoreCRefArray const & semaphoresToWait )const
	{
		assert( swapChains.size() == imagesIndex.size() );
		auto vkSwapchains = makeVkArray< VkSwapchainKHR >( swapChains );
		auto vkSemaphoresToWait = makeVkArray< VkSemaphore >( semaphoresToWait );
		std::vector< VkResult > results;
		results.resize( swapChains.size() );
		VkPresentInfoKHR presentInfo
		{
			VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			nullptr,
			static_cast< uint32_t >( vkSemaphoresToWait.size() ),    // waitSemaphoreCount
			vkSemaphoresToWait.data(),                               // pWaitSemaphores
			static_cast< uint32_t >( imagesIndex.size() ),           // swapchainCount
			vkSwapchains.data(),                                     // pSwapchains
			imagesIndex.data(),                                      // pImageIndices
			results.data()                                           // pResults
		};
		DEBUG_DUMP( presentInfo );
		checkError( m_device.vkQueuePresentKHR( m_internal, &presentInfo )
			, "Queue present" );
		return results;
	}

	void Queue::waitIdle()const
	{
		auto res = m_device.vkQueueWaitIdle( m_internal );
		checkError( res, "Queue wait idle" );
	}

#if VK_EXT_debug_utils

	void Queue::beginDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const
	{
		if ( m_device.vkQueueBeginDebugUtilsLabelEXT )
		{
			m_device.vkQueueBeginDebugUtilsLabelEXT( m_internal, &labelInfo );
		}
	}

	void Queue::endDebugUtilsLabel()const
	{
		if ( m_device.vkQueueEndDebugUtilsLabelEXT )
		{
			m_device.vkQueueEndDebugUtilsLabelEXT( m_internal );
		}
	}

	void Queue::insertDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const
	{
		if ( m_device.vkQueueInsertDebugUtilsLabelEXT )
		{
			m_device.vkQueueInsertDebugUtilsLabelEXT( m_internal, &labelInfo );
		}
	}

#endif
}
