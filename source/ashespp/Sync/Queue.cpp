/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Sync/Queue.hpp"

#include "ashespp/Command/CommandBuffer.hpp"
#include "ashespp/Core/Device.hpp"
#include "ashespp/Core/SwapChain.hpp"
#include "ashespp/Sync/Fence.hpp"
#include "ashespp/Sync/Semaphore.hpp"
#include "ashespp/Sync/SyncException.hpp"

namespace ashes
{
	Queue::Queue( Device const & device
		, uint32_t familyIndex
		, uint32_t index )
		: Queue{ device, "Queue", familyIndex, index }
	{
	}

	Queue::Queue( Device const & device
		, std::string const & debugName
		, uint32_t familyIndex
		, uint32_t index )
		: VkObject{ debugName }
		, m_device{ device }
		, m_familyIndex{ familyIndex }
		, m_index{ index }
	{
		m_device.vkGetDeviceQueue( m_device
			, m_familyIndex
			, m_index
			, &m_internal );
		registerObject( m_device, debugName, *this );
	}

	Queue::~Queue()
	{
		unregisterObject( m_device, *this );
	}

	VkResult Queue::present( VkSwapchainKHR swapChain
		, uint32_t imageIndex
		, VkSemaphore semaphoreToWait )const
	{
		UInt32Array indices;
		indices.push_back( imageIndex );
		return present( VkSwapchainArrayKHR{ swapChain }
			, indices
			, VkSemaphoreArray{ semaphoreToWait } ).front();
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

	VkResult Queue::present( VkSwapchainKHR swapChain
		, uint32_t imageIndex )const
	{
		UInt32Array indices;
		indices.push_back( imageIndex );
		return present( VkSwapchainArrayKHR{ swapChain }
			, indices
			, VkSemaphoreArray{} ).front();
	}

	VkResult Queue::present( SwapChain const & swapChain
		, uint32_t imageIndex )const
	{
		UInt32Array indices;
		indices.push_back( imageIndex );
		return present( SwapChainCRefArray{ { std::ref( swapChain ) } }
			, indices
			, SemaphoreCRefArray{} ).front();
	}

	void Queue::submit( VkCommandBufferArray const & commandBuffers
		, VkSemaphoreArray const & semaphoresToWait
		, VkPipelineStageFlagsArray const & semaphoresStage
		, VkSemaphoreArray const & semaphoresToSignal
		, VkFence fence )const
	{
		std::vector< VkSubmitInfo > submitInfo{ { VK_STRUCTURE_TYPE_SUBMIT_INFO
			, nullptr
			, static_cast< uint32_t >( semaphoresToWait.size() )      // waitSemaphoreCount
			, ( semaphoresToWait.empty()                              // pWaitSemaphores
				? nullptr
				: semaphoresToWait.data() )
			, ( semaphoresStage.empty()                               // pWaitDstStageMask
				? nullptr
				: semaphoresStage.data() )
			, static_cast< uint32_t >( commandBuffers.size() )        // commandBufferCount
			, ( commandBuffers.empty()                                // pCommandBuffers
				? nullptr
				: commandBuffers.data() )
			, static_cast< uint32_t >( semaphoresToSignal.size() )    // signalSemaphoreCount
			, ( semaphoresToSignal.empty()                            // pSignalSemaphores
				? nullptr
				: semaphoresToSignal.data() ) } };
		DEBUG_DUMP( submitInfo );
		auto res = m_device.vkQueueSubmit( m_internal
			, static_cast< uint32_t >( submitInfo.size() )
			, submitInfo.data()
			, fence );
		checkError( res, "Queue submit" );
	}

	void Queue::submit( VkCommandBuffer commandBuffers
		, VkSemaphoreArray const & semaphoresToWait
		, VkPipelineStageFlagsArray const & semaphoresStage
		, VkSemaphore semaphoresToSignal
		, VkFence fence )const
	{
		submit( { 1u, commandBuffers }
			, semaphoresToWait
			, semaphoresStage
			, ( semaphoresToSignal
				? VkSemaphoreArray{ 1u, semaphoresToSignal }
				: VkSemaphoreArray{} )
			, fence );
	}

	void Queue::submit( CommandBufferCRefArray const & commandBuffers
		, SemaphoreCRefArray const & semaphoresToWait
		, VkPipelineStageFlagsArray const & semaphoresStage
		, SemaphoreCRefArray const & semaphoresToSignal
		, Fence const * fence )const
	{
		waitSemaphores( semaphoresToWait );
		submit( makeVkArray< VkCommandBuffer >( commandBuffers )
			, makeVkArray< VkSemaphore >( semaphoresToWait )
			, semaphoresStage
			, makeVkArray< VkSemaphore >( semaphoresToSignal )
			, fence ? VkFence( *fence ) : VkFence{} );
		signalSemaphores( semaphoresToSignal, fence );
	}

	VkResultArray Queue::present( VkSwapchainArrayKHR const & swapchains
		, UInt32Array const & imagesIndex
		, VkSemaphoreArray const & semaphoresToWait )const
	{
		assert( swapchains.size() == imagesIndex.size() );
		std::vector< VkResult > results;
		results.resize( swapchains.size() );
		VkPresentInfoKHR presentInfo
		{
			VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			nullptr,
			static_cast< uint32_t >( semaphoresToWait.size() ),    // waitSemaphoreCount
			semaphoresToWait.data(),                               // pWaitSemaphores
			static_cast< uint32_t >( imagesIndex.size() ),         // swapchainCount
			swapchains.data(),                                     // pSwapchains
			imagesIndex.data(),                                    // pImageIndices
			results.data()                                         // pResults
		};
		DEBUG_DUMP( presentInfo );
		checkError( m_device.vkQueuePresentKHR( m_internal, &presentInfo )
			, "Queue present" );
		return results;
	}

	VkResultArray Queue::present( SwapChainCRefArray const & swapChains
		, UInt32Array const & imagesIndex
		, SemaphoreCRefArray const & semaphoresToWait )const
	{
		waitSemaphores( semaphoresToWait );
		return present( makeVkArray< VkSwapchainKHR >( swapChains )
			, imagesIndex
			, makeVkArray< VkSemaphore >( semaphoresToWait ) );
	}

	void Queue::waitIdle()const
	{
		clearSemaphores();
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

	void Queue::clearSemaphores()const
	{
#if Ashes_DebugSync
		auto it = m_waitingSemaphores.begin();
		auto end = m_waitingSemaphores.end();
		auto size = m_waitingSemaphores.size();
		auto index = 0u;

		while ( it != end )
		{
			( *it )->signal( nullptr );

			if ( size != m_waitingSemaphores.size() )
			{
				size = m_waitingSemaphores.size();
				it = std::next( m_waitingSemaphores.begin(), index );
			}
			else
			{
				++it;
				++index;
			}
		}

		m_waitingSemaphores.clear();
#endif
	}

	void Queue::waitSemaphores( SemaphoreCRefArray const & semaphores )const
	{
#if Ashes_DebugSync
		for ( auto & semaphore : semaphores )
		{
			semaphore.get().wait( m_waitingSemaphores );
			auto pair = m_waitingSemaphores.insert( &semaphore.get() );
			ashesSyncCheck( pair.second
				, "The same semaphore is being submitted twice"
				, semaphore.get() );
		}
#endif
	}

	void Queue::signalSemaphores( SemaphoreCRefArray const & semaphores
		, Fence const * fence )const
	{
#if Ashes_DebugSync
		for ( auto & semaphore : semaphores )
		{
			semaphore.get().signal( fence );
		}
#endif
	}
}
