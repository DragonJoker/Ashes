/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <Command/Queue.hpp>

namespace vk_renderer
{
	class Queue
		: public ashes::Queue
	{
	public:
		Queue( Device const & device
			, uint32_t familyIndex );
		/**
		*\~french
		*\brief
		*	Présente la file à Vulkan.
		*\return
		*	\p true si tout s'est bien passé.
		*\~english
		*\brief
		*	present the queue to Vulkan.
		*\return
		*	\p true on ok.
		*/
		VkResult presentBackBuffer( SwapChainCRefArray const & swapChains
			, ashes::UInt32Array const & imagesIndex
			, SemaphoreCRefArray const & semaphoresToWait )const;
		/**
		*\copydoc		ashes::Queue::submit
		*/ 
		void submit( ashes::CommandBufferCRefArray const & commandBuffers
			, ashes::SemaphoreCRefArray const & semaphoresToWait
			, ashes::PipelineStageFlagsArray const & semaphoresStage
			, ashes::SemaphoreCRefArray const & semaphoresToSignal
			, ashes::Fence const * fence )const override;
		/**
		*\copydoc		ashes::Queue::present
		*/
		void present( ashes::SwapChainCRefArray const & swapChains
			, ashes::UInt32Array const & imagesIndex
			, ashes::SemaphoreCRefArray const & semaphoresToWait )const override;
		/**
		/**
		*\copydoc		ashes::Queue::waitIdle
		*/
		void waitIdle()const override;
		/**
		/**
		*\copydoc		ashes::Queue::getFamilyIndex
		*/
		inline uint32_t getFamilyIndex()const override
		{
			return m_familyIndex;
		}
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkQueue.
		*\~english
		*\brief
		*	VkQueue implicit cast operator.
		*/
		inline operator VkQueue const &()const
		{
			return m_queue;
		}

	private:
		Device const & m_device;
		VkQueue m_queue{ VK_NULL_HANDLE };
		uint32_t m_familyIndex{ 0u };
	};
}
