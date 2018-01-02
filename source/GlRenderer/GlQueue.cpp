/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "GlQueue.hpp"

#include "GlCommandBuffer.hpp"
#include "GlDevice.hpp"
#include "GlFence.hpp"
#include "GlSemaphore.hpp"
#include "GlSwapChain.hpp"
#include "Commands/GlCommandBase.hpp"

namespace gl_renderer
{
	Queue::Queue()
	{
	}

	bool Queue::submit( renderer::CommandBuffer const & commandBuffer
		, renderer::Fence const * fence )const
	{
		for ( auto & command : static_cast< CommandBuffer const & >( commandBuffer ).getCommands() )
		{
			command->apply();
		}

		return fence
			? fence->wait( ~( 0u ) ) == renderer::WaitResult::eSuccess
			: true;
	}

	bool Queue::submit( renderer::CommandBuffer const & commandBuffer
		, renderer::Semaphore const & semaphoreToWait
		, renderer::PipelineStageFlags const & semaphoreStage
		, renderer::Semaphore const & semaphoreToSignal
		, renderer::Fence const * fence )const
	{
		for ( auto & command : static_cast< CommandBuffer const & >( commandBuffer ).getCommands() )
		{
			command->apply();
		}

		return fence
			? fence->wait( ~( 0u ) ) == renderer::WaitResult::eSuccess
			: true;
	}

	bool Queue::submit( renderer::CommandBufferCRefArray const & commandBuffers
		, renderer::SemaphoreCRefArray const & semaphoresToWait
		, renderer::PipelineStageFlagsArray const & semaphoresStage
		, renderer::SemaphoreCRefArray const & semaphoresToSignal
		, renderer::Fence const * fence )const
	{
		for ( auto & commandBuffer : commandBuffers )
		{
			for ( auto & command : static_cast< CommandBuffer const & >( commandBuffer.get() ).getCommands() )
			{
				command->apply();
			}
		}

		return fence
			? fence->wait( ~( 0u ) ) == renderer::WaitResult::eSuccess
			: true;
	}

	bool Queue::present( renderer::SwapChainCRefArray const & swapChains
		, renderer::UInt32Array const & imagesIndex
		, renderer::SemaphoreCRefArray const & semaphoresToWait )const
	{
		return true;
		//return vk::checkError( m_queue.present( convert( swapChains )
		//	, imagesIndex
		//	, convert( semaphoresToWait ) ) );
	}

	bool Queue::waitIdle()const
	{
		glFinish();
		return true;
	}
}
