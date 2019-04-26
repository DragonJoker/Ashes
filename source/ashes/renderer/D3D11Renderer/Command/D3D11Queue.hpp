/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	class Queue
	{
	public:
		Queue( VkDevice device
			, VkDeviceQueueCreateInfo createInfo
			, uint32_t index );
		~Queue();
		/**
		*\copydoc		ashes::Queue::submit
		*/ 
		VkResult submit( VkSubmitInfoArray const & infos
			, VkFence fence )const;
		/**
		*\copydoc		Queue::present
		*/
		VkResult present( VkPresentInfoKHR const & presentInfo )const;
		/**
		/**
		*\copydoc		ashes::Queue::waitIdle
		*/
		VkResult waitIdle()const;

	private:
		VkResult doSubmit( VkCommandBufferArray const & commandBuffers
			, VkSemaphoreArray const & semaphoresToWait
			, VkPipelineStageFlagsArray const & semaphoresStage
			, VkSemaphoreArray const & semaphoresToSignal
			, VkFence fence )const;

	private:
		VkDevice m_device;
		VkDeviceQueueCreateInfo m_createInfo;
		uint32_t m_index;
		ID3D11Query * m_waitIdleQuery{ nullptr };
	};
}
