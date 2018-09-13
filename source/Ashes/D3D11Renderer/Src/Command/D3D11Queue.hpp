/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "D3D11RendererPrerequisites.hpp"

#include <Command/Queue.hpp>

namespace d3d11_renderer
{
	class Queue
		: public ashes::Queue
	{
	public:
		Queue( Device const & device
			, uint32_t familyIndex );
		~Queue();
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

	private:
		Device const & m_device;
		uint32_t m_familyIndex{ 0u };
		ID3D11Query * m_waitIdleQuery{ nullptr };
	};
}
