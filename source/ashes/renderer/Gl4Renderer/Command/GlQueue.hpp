/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"
#include "renderer/Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
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
		void submit( ContextLock & context
			, VkSubmitInfo const & value
			, VkFence fence )const;

	private:
		VkDevice m_device;
		VkDeviceQueueCreateInfo m_createInfo;
		uint32_t m_index;
	};
}
