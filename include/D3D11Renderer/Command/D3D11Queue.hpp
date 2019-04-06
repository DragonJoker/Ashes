/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/Command/Queue.hpp>

namespace d3d11_renderer
{
	class Queue
		: public ashes::Queue
	{
	public:
		Queue( Device const & device
			, ashes::DeviceQueueCreateInfo createInfo
			, uint32_t index );
		~Queue();
		/**
		*\copydoc		ashes::Queue::submit
		*/ 
		void submit( ashes::CommandBufferCRefArray const & commandBuffers
			, ashes::SemaphoreCRefArray const & semaphoresToWait
			, ashes::VkPipelineStageFlagsArray const & semaphoresStage
			, ashes::SemaphoreCRefArray const & semaphoresToSignal
			, ashes::Fence const * fence )const override;
		/**
		*\copydoc		ashes::Queue::present
		*/
		ashes::VkResultArray present( ashes::SwapChainCRefArray const & swapChains
			, ashes::UInt32Array const & imagesIndex
			, ashes::SemaphoreCRefArray const & semaphoresToWait )const override;
		/**
		/**
		*\copydoc		ashes::Queue::waitIdle
		*/
		void waitIdle()const override;

	private:
		Device const & m_device;
		ID3D11Query * m_waitIdleQuery{ nullptr };
	};
}
