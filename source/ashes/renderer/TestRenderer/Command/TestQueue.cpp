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
		, ashes::VkPipelineStageFlagsArray const & semaphoresStage
		, ashes::SemaphoreCRefArray const & semaphoresToSignal
		, ashes::Fence const * fence )const
	{
	}

	ashes::VkResultArray Queue::present( ashes::SwapChainCRefArray const & swapChains
		, ashes::UInt32Array const & imagesIndex
		, ashes::SemaphoreCRefArray const & semaphoresToWait )const
	{
		ashes::VkResultArray result{ swapChains.size(), ashes::Result::eSuccess };

		for ( auto & swapChain : swapChains )
		{
		}

		return result;
	}

	void Queue::waitIdle()const
	{
	}
}
