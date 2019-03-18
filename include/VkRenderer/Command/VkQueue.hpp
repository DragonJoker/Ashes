/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "VkRenderer/VkRendererPrerequisites.hpp"

#include <Ashes/Command/Queue.hpp>

namespace vk_renderer
{
	class Queue
		: public ashes::Queue
	{
	public:
		Queue( Device const & device
			, ashes::DeviceQueueCreateInfo createInfo
			, uint32_t index );
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
		ashes::ResultArray present( ashes::SwapChainCRefArray const & swapChains
			, ashes::UInt32Array const & imagesIndex
			, ashes::SemaphoreCRefArray const & semaphoresToWait )const override;
		/**
		/**
		*\copydoc		ashes::Queue::waitIdle
		*/
		void waitIdle()const override;
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
	};
}
