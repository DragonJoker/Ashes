/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Command/TestQueue.hpp"

#include "Command/TestCommandBuffer.hpp"
#include "Core/TestDevice.hpp"
#include "Core/TestSwapChain.hpp"
#include "Sync/TestFence.hpp"
#include "Sync/TestSemaphore.hpp"

namespace test_renderer
{
	Queue::Queue( Device const & device
		, uint32_t familyIndex )
		: renderer::Queue{ device }
		, m_familyIndex{ familyIndex }
	{
	}

	bool Queue::submit( renderer::CommandBufferCRefArray const & commandBuffers
		, renderer::SemaphoreCRefArray const & semaphoresToWait
		, renderer::PipelineStageFlagsArray const & semaphoresStage
		, renderer::SemaphoreCRefArray const & semaphoresToSignal
		, renderer::Fence const * fence )const
	{
		return true;
	}

	bool Queue::present( renderer::SwapChainCRefArray const & swapChains
		, renderer::UInt32Array const & imagesIndex
		, renderer::SemaphoreCRefArray const & semaphoresToWait )const
	{
		return true;
	}

	bool Queue::waitIdle()const
	{
		return true;
	}
}
