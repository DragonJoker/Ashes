/*
This file belongs to Ashes.
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
		, ashes::DeviceQueueCreateInfo createInfo
		, uint32_t index )
		: ashes::Queue{ device, std::move( createInfo ), index }
	{
	}

	void Queue::submit( ashes::CommandBufferCRefArray const & commandBuffers
		, ashes::SemaphoreCRefArray const & semaphoresToWait
		, ashes::PipelineStageFlagsArray const & semaphoresStage
		, ashes::SemaphoreCRefArray const & semaphoresToSignal
		, ashes::Fence const * fence )const
	{
	}

	void Queue::present( ashes::SwapChainCRefArray const & swapChains
		, ashes::UInt32Array const & imagesIndex
		, ashes::SemaphoreCRefArray const & semaphoresToWait )const
	{
	}

	void Queue::waitIdle()const
	{
	}
}
