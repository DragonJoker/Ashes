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

namespace gl_renderer
{
	//namespace
	//{
	//	vk::CommandBufferCRefArray convert( renderer::CommandBufferCRefArray const & values )
	//	{
	//		vk::CommandBufferCRefArray result;
	//		result.reserve( values.size() );

	//		for ( auto & value : values )
	//		{
	//			result.emplace_back( static_cast< CommandBuffer const & >( value.get() ).getCommandBuffer() );
	//		}

	//		return result;
	//	}

	//	vk::SemaphoreCRefArray convert( renderer::SemaphoreCRefArray const & values )
	//	{
	//		vk::SemaphoreCRefArray result;
	//		result.reserve( values.size() );

	//		for ( auto & value : values )
	//		{
	//			result.emplace_back( static_cast< Semaphore const & >( value.get() ).getSemaphore() );
	//		}

	//		return result;
	//	}

	//	vk::SwapChainCRefArray convert( renderer::SwapChainCRefArray const & values )
	//	{
	//		vk::SwapChainCRefArray result;
	//		result.reserve( values.size() );

	//		for ( auto & value : values )
	//		{
	//			result.emplace_back( static_cast< SwapChain const & >( value.get() ).getSwapChain() );
	//		}

	//		return result;
	//	}
	//}

	Queue::Queue()
	{
	}

	bool Queue::submit( renderer::CommandBuffer const & commandBuffer
		, renderer::Fence const * fence )const
	{
		return false;
		//return vk::checkError( m_queue.submit( static_cast< CommandBuffer const & >( commandBuffer ).getCommandBuffer()
		//	, fence ? &static_cast< Fence const * >( fence )->getFence() : nullptr ) );
	}

	bool Queue::submit( renderer::CommandBuffer const & commandBuffer
		, renderer::Semaphore const & semaphoreToWait
		, renderer::PipelineStageFlags const & semaphoreStage
		, renderer::Semaphore const & semaphoreToSignal
		, renderer::Fence const * fence )const
	{
		return false;
		//return vk::checkError( m_queue.submit( static_cast< CommandBuffer const & >( commandBuffer ).getCommandBuffer()
		//	, static_cast< Semaphore const & >( semaphoreToWait ).getSemaphore()
		//	, semaphoreStage
		//	, static_cast< Semaphore const & >( semaphoreToSignal ).getSemaphore()
		//	, fence ? &static_cast< Fence const * >( fence )->getFence() : nullptr ) );
	}

	bool Queue::submit( renderer::CommandBufferCRefArray const & commandBuffers
		, renderer::SemaphoreCRefArray const & semaphoresToWait
		, renderer::PipelineStageFlagsArray const & semaphoresStage
		, renderer::SemaphoreCRefArray const & semaphoresToSignal
		, renderer::Fence const * fence )const
	{
		return false;
		//return vk::checkError( m_queue.submit( convert( commandBuffers )
		//	, convert( semaphoresToWait )
		//	, convert< VkPipelineStageFlags >( semaphoresStage )
		//	, convert( semaphoresToSignal )
		//	, fence ? &static_cast< Fence const * >( fence )->getFence() : nullptr ) );
	}

	bool Queue::present( renderer::SwapChainCRefArray const & swapChains
		, renderer::UInt32Array const & imagesIndex
		, renderer::SemaphoreCRefArray const & semaphoresToWait )const
	{
		return false;
		//return vk::checkError( m_queue.present( convert( swapChains )
		//	, imagesIndex
		//	, convert( semaphoresToWait ) ) );
	}

	bool Queue::waitIdle()const
	{
		return false;
		//return vk::checkError( m_queue.waitIdle() );
	}
}
