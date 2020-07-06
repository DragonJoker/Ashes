/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Queue_HPP___
#define ___AshesPP_Queue_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	/**
	*\brief
	*	Queue wrapper.
	*/
	class Queue
	{
	public:
		Queue( Device const & device
			, uint32_t familyIndex
			, uint32_t index );
		/**
		*\brief
		*	Submits given command buffers.
		*\param[in] commandBuffers
		*	The command buffers.
		*\param[in] semaphoresToWait
		*	The semaphores to wait.
		*\param[in] semaphoresStage
		*	The semaphores respective stages.
		*\param[in] semaphoresToSignal
		*	The semaphores to signal.
		*\param[in] fence
		*	An optional fence.
		*\return
		*	\p true on ok.
		*/ 
		void submit( CommandBufferCRefArray const & commandBuffers
			, SemaphoreCRefArray const & semaphoresToWait
			, VkPipelineStageFlagsArray const & semaphoresStage
			, SemaphoreCRefArray const & semaphoresToSignal
			, Fence const * fence )const;
		/**
		*\brief
		*	Presents the swapchains.
		*\param[in] swapChains
		*	The swapchains.
		*\param[in] imagesIndex
		*	The image to present for each swapchain.
		*\param[in] semaphoresToWait
		*	The semaphore to wait for each presented swapchain.
		*\return
		*	The result for each swapchain presentation.
		*/ 
		VkResultArray present( SwapChainCRefArray const & swapChains
			, UInt32Array const & imagesIndex
			, SemaphoreCRefArray const & semaphoresToWait )const;
		/**
		*\brief
		*	Presents a swapchain.
		*\param[in] swapChain
		*	The swapchain.
		*\param[in] imageIndex
		*	The image to present.
		*\param[in] semaphoreToWait
		*	The semaphore to wait.
		*\return
		*	The presentation result.
		*/
		VkResult present( SwapChain const & swapChain
			, uint32_t imageIndex
			, Semaphore const & semaphoreToWait )const;
		/**
		*\brief
		*	Presents a swapchain.
		*\param[in] swapChain
		*	The swapchain.
		*\param[in] imageIndex
		*	The image to present.
		*\param[in] semaphoreToWait
		*	The semaphore to wait.
		*\return
		*	The presentation result.
		*/
		VkResult present( SwapChain const & swapChain
			, uint32_t imageIndex )const;
		/**
		*\brief
		*	Waits for the queue to be idle.
		*\return
		*	\p true on ok.
		*/
		void waitIdle()const;
#if VK_EXT_debug_utils
		/**
		*\brief
		*	Begins a queue label.
		*\param[in] labelInfo
		*	The parameters of the label to begin.
		*/
		void beginDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const;
		/**
		*\brief
		*	Ends the queue label.
		*/
		void endDebugUtilsLabel()const;
		/**
		*\brief
		*	Inserts a queue label.
		*\param[in] labelInfo
		*	The parameters of the label to begin.
		*/
		void insertDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const;
#endif
		/**
		*\return
		*	The queue family index.
		*/
		inline uint32_t getFamilyIndex()const
		{
			return m_familyIndex;
		}
		/**
		*\return
		*	The queue index within its family.
		*/
		inline uint32_t getIndex()const
		{
			return m_index;
		}
		/**
		*\brief
		*	Submits given command buffer.
		*\param[in] commandBuffer
		*	The command buffer.
		*\param[in] fence
		*	An optional fence.
		*\return
		*	\p true on ok.
		*/
		inline void submit( CommandBuffer const & commandBuffer
			, Fence const * fence )const
		{
			submit( { commandBuffer }
				, SemaphoreCRefArray{}
				, VkPipelineStageFlagsArray{}
				, SemaphoreCRefArray{}
				, fence );
		}
		/**
		*\brief
		*	Submits given command buffer.
		*\param[in] commandBuffer
		*	The command buffer.
		*\param[in] semaphoreToWait
		*	The semaphore to wait.
		*\param[in] semaphoreStage
		*	The semaphore respective stages.
		*\param[in] semaphoreToSignal
		*	The semaphore to signal.
		*\param[in] fence
		*	An optional fence.
		*\return
		*	\p true on ok.
		*/
		inline void submit( CommandBuffer const & commandBuffer
			, Semaphore const & semaphoreToWait
			, VkPipelineStageFlags const & semaphoreStage
			, Semaphore const & semaphoreToSignal
			, Fence const * fence )const
		{
			submit( { commandBuffer }
				, SemaphoreCRefArray{ semaphoreToWait }
				, VkPipelineStageFlagsArray{ semaphoreStage }
				, SemaphoreCRefArray{ semaphoreToSignal }
				, fence );
		}
		/**
		*\brief
		*	VkQueue implicit cast operator.
		*/
		inline operator VkQueue const &()const
		{
			return m_internal;
		}

	private:
		void clearSemaphores()const;
		void waitSemaphores( SemaphoreCRefArray const & semaphores )const;
		void signalSemaphores( SemaphoreCRefArray const & semaphores
			, Fence const * fence )const;

	private:
		Device const & m_device;
		uint32_t m_familyIndex;
		uint32_t m_index;
		VkQueue m_internal{ VK_NULL_HANDLE };
		mutable std::set< Semaphore const * > m_waitingSemaphores;
	};
}

#endif
