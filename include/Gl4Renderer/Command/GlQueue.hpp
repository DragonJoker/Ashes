/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Command/Queue.hpp>

namespace gl_renderer
{
	class Queue
	{
	public:
		Queue( Device const & device
			, ashes::DeviceQueueCreateInfo createInfo
			, uint32_t index );
		/**
		*\copydoc		ashes::Queue::submit
		*/
		void submit( std::vector< VkSubmitInfo > values
			, VkFence fence )const;
		/**
		*\copydoc		ashes::Queue::present
		*/
		ashes::ResultArray present( ashes::SwapChainCRefArray const & swapChains
			, ashes::UInt32Array const & imagesIndex
			, ashes::SemaphoreCRefArray const & semaphoresToWait )const;
		/**
		*\copydoc		ashes::Queue::waitIdle
		*/
		void waitIdle()const;

	private:
		Device const & m_device;
	};
}
