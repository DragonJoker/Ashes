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
		*\copydoc		ashes::Queue::waitIdle
		*/
		VkResult waitIdle()const;
#if VK_EXT_debug_utils
		void beginDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const;
		void endDebugUtilsLabel()const;
		void insertDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const;
#endif
#if VK_EXT_debug_marker
		void debugMarkerBegin( VkDebugMarkerMarkerInfoEXT const & labelInfo )const;
		void debugMarkerEnd()const;
		void debugMarkerInsert( VkDebugMarkerMarkerInfoEXT const & labelInfo )const;
#endif

		inline VkDevice getDevice()const
		{
			return m_device;
		}

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
		mutable Optional< DebugLabel > m_label;
	};
}
