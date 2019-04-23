/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	class Queue
	{
	public:
		Queue( VkDevice device
			, VkDeviceQueueCreateInfo createInfo
			, uint32_t index );
		/**
		*\copydoc		Queue::submit
		*/
		VkResult submit( VkSubmitInfoArray const & values
			, VkFence fence )const;
		/**
		*\copydoc		Queue::present
		*/
		VkResult present( VkPresentInfoKHR const & presentInfo )const;
		/**
		*\copydoc		Queue::waitIdle
		*/
		VkResult waitIdle()const;

	private:
		void submit( VkSubmitInfo const & value
			, VkFence fence )const;

	private:
		VkDevice m_device;
		VkDeviceQueueCreateFlags m_flags;
		uint32_t m_queueFamilyIndex;
		float m_queuePriority;
		uint32_t m_index;
	};
}
