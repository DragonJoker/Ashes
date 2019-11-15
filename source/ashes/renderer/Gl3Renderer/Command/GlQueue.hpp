/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/Gl3Renderer/GlRendererPrerequisites.hpp"
#include "renderer/Gl3Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl3
{
	void applyBuffer( ContextLock const & lock
		, CmdBuffer const & cmds );
	void applyList( ContextLock const & lock
		, CmdList const & cmds );

	class Queue
	{
	public:
		Queue( VkDevice device
			, VkDeviceQueueCreateInfo createInfo
			, uint32_t index );

		VkResult submit( VkSubmitInfoArray const & values
			, VkFence fence )const;
		VkResult present( VkPresentInfoKHR const & presentInfo )const;
		VkResult waitIdle()const;
#if VK_EXT_debug_utils
		void beginDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const;
		void endDebugUtilsLabel()const;
		void insertDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const;
#endif

		inline VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		void submit( ContextLock & context
			, VkSubmitInfo const & value
			, VkFence fence )const;

	private:
		VkDevice m_device;
		VkDeviceQueueCreateInfo m_createInfo;
		uint32_t m_index;
		mutable Optional< DebugLabel > m_label;
	};
}
