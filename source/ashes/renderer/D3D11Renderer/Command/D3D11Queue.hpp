/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/Command/Queue.hpp>

namespace ashes::d3d11
{
	class Queue
		: public ashes::Queue
	{
	public:
		Queue( VkDevice device
			, VkDeviceQueueCreateInfo createInfo
			, uint32_t index );
		~Queue();
		/**
		*\copydoc		ashes::Queue::submit
		*/ 
		void submit( ashes::CommandBufferCRefArray const & commandBuffers
			, ashes::SemaphoreCRefArray const & semaphoresToWait
			, ashes::VkPipelineStageFlagsArray const & semaphoresStage
			, ashes::SemaphoreCRefArray const & semaphoresToSignal
			, ashes::Fence const * fence )const;
		/**
		*\copydoc		ashes::Queue::present
		*/
		ashes::VkResultArray present( ashes::SwapChainCRefArray const & swapChains
			, ashes::UInt32Array const & imagesIndex
			, ashes::SemaphoreCRefArray const & semaphoresToWait )const;
		/**
		/**
		*\copydoc		ashes::Queue::waitIdle
		*/
		void waitIdle()const;

	private:
		VkDevice m_device;
		ID3D11Query * m_waitIdleQuery{ nullptr };
	};
}
