/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Command/Queue.hpp>

namespace test_renderer
{
	class Queue
		: public renderer::Queue
	{
	public:
		Queue( Device const & device
			, uint32_t familyIndex );
		/**
		*\copydoc		renderer::Queue::submit
		*/ 
		void submit( renderer::CommandBufferCRefArray const & commandBuffers
			, renderer::SemaphoreCRefArray const & semaphoresToWait
			, renderer::PipelineStageFlagsArray const & semaphoresStage
			, renderer::SemaphoreCRefArray const & semaphoresToSignal
			, renderer::Fence const * fence )const override;
		/**
		*\copydoc		renderer::Queue::present
		*/
		void present( renderer::SwapChainCRefArray const & swapChains
			, renderer::UInt32Array const & imagesIndex
			, renderer::SemaphoreCRefArray const & semaphoresToWait )const override;
		/**
		/**
		*\copydoc		renderer::Queue::waitIdle
		*/
		void waitIdle()const override;
		/**
		/**
		*\copydoc		renderer::Queue::getFamilyIndex
		*/
		inline uint32_t getFamilyIndex()const override
		{
			return m_familyIndex;
		}

	private:
		uint32_t m_familyIndex{ 0u };
	};
}
