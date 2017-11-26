/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "Queue.hpp"

#include "CommandBuffer.hpp"
#include "Device.hpp"
#include "Fence.hpp"
#include "Semaphore.hpp"
#include "SwapChain.hpp"

namespace renderer
{
	namespace
	{
		vk::PrimaryCommandBufferCRefArray convert( CommandBufferCRefArray const & values )
		{
			vk::PrimaryCommandBufferCRefArray result;
			result.reserve( values.size() );

			for ( auto & value : values )
			{
				result.emplace_back( value.get().getCommandBuffer() );
			}

			return result;
		}

		vk::SemaphoreCRefArray convert( SemaphoreCRefArray const & values )
		{
			vk::SemaphoreCRefArray result;
			result.reserve( values.size() );

			for ( auto & value : values )
			{
				result.emplace_back( value.get().getSemaphore() );
			}

			return result;
		}

		vk::SwapChainCRefArray convert( SwapChainCRefArray const & values )
		{
			vk::SwapChainCRefArray result;
			result.reserve( values.size() );

			for ( auto & value : values )
			{
				result.emplace_back( value.get().getSwapChain() );
			}

			return result;
		}
	}

	Queue::Queue( vk::Queue const & queue )
		: m_queue{ queue }
	{
	}

	bool Queue::submit( CommandBuffer const & commandBuffer
		, Fence const * fence )const
	{
		return vk::checkError( m_queue.submit( commandBuffer.getCommandBuffer()
			, fence ? &fence->getFence() : nullptr ) );
	}

	bool Queue::submit( CommandBuffer const & commandBuffer
		, Semaphore const & semaphoreToWait
		, PipelineStageFlags const & semaphoreStage
		, Semaphore const & semaphoreToSignal
		, Fence const * fence )const
	{
		return vk::checkError( m_queue.submit( commandBuffer.getCommandBuffer()
			, semaphoreToWait.getSemaphore()
			, semaphoreStage
			, semaphoreToSignal.getSemaphore()
			, fence ? &fence->getFence() : nullptr ) );
	}

	bool Queue::submit( CommandBufferCRefArray const & commandBuffers
		, SemaphoreCRefArray const & semaphoresToWait
		, PipelineStageFlagsArray const & semaphoresStage
		, SemaphoreCRefArray const & semaphoresToSignal
		, Fence const * fence )const
	{
		return vk::checkError( m_queue.submit( convert( commandBuffers )
			, convert( semaphoresToWait )
			, convert< VkPipelineStageFlags >( semaphoresStage )
			, convert( semaphoresToSignal )
			, fence ? &fence->getFence() : nullptr ) );
	}

	bool Queue::present( SwapChainCRefArray const & swapChains
		, UInt32Array const & imagesIndex
		, SemaphoreCRefArray const & semaphoresToWait )const
	{
		return vk::checkError( m_queue.present( convert( swapChains )
			, imagesIndex
			, convert( semaphoresToWait ) ) );
	}
}
