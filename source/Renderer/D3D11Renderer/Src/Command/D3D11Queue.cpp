/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Command/D3D11Queue.hpp"

#include "Command/Commands/D3D11CommandBase.hpp"
#include "Command/D3D11CommandBuffer.hpp"
#include "Core/D3D11Device.hpp"
#include "Core/D3D11SwapChain.hpp"
#include "Sync/D3D11Fence.hpp"
#include "Sync/D3D11Semaphore.hpp"

namespace d3d11_renderer
{
	Queue::Queue( Device const & device
		, uint32_t familyIndex
		, D3D11_CONTEXT_TYPE queueType )
		: renderer::Queue{ device }
		, m_device{ device }
		, m_familyIndex{ familyIndex }
		, m_queueType{ queueType }
		, m_idleEvent{ ::CreateEvent( nullptr, TRUE, FALSE, nullptr ) }
	{
	}
	
	Queue::~Queue()
	{
		::CloseHandle( m_idleEvent );
	}

	void Queue::submit( renderer::CommandBufferCRefArray const & commandBuffers
		, renderer::SemaphoreCRefArray const & semaphoresToWait
		, renderer::PipelineStageFlagsArray const & semaphoresStage
		, renderer::SemaphoreCRefArray const & semaphoresToSignal
		, renderer::Fence const * fence )const
	{
		Context context{ m_device };

		for ( auto & commandBuffer : commandBuffers )
		{
			auto & dxCommandBuffer = static_cast< CommandBuffer const & >( commandBuffer.get() );
			dxCommandBuffer.execute( context );
			context.uavs.clear();
		}

#if Renderer_HasFence

		if ( fence )
		{
			context.context4->Signal( static_cast< Fence const & >( *fence ).getFence(), 1u );
		}

#endif
	}

	void Queue::present( renderer::SwapChainCRefArray const & swapChains
		, renderer::UInt32Array const & imagesIndex
		, renderer::SemaphoreCRefArray const & semaphoresToWait )const
	{
		for ( auto & swapChain : swapChains )
		{
		}
	}

	void Queue::waitIdle()const
	{
		ID3D11DeviceContext3 * context;
		m_device.getDevice3()->GetImmediateContext3( &context );
		context->Flush1( m_queueType, m_idleEvent );
		::WaitForSingleObject( m_idleEvent, INFINITE );
		safeRelease( context );
	}
}
