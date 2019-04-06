/*
This file belongs to Ashes.
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
		, ashes::DeviceQueueCreateInfo createInfo
		, uint32_t index )
		: ashes::Queue{ device, std::move( createInfo ), index }
		, m_device{ device }
	{
		D3D11_QUERY_DESC desc{};
		desc.Query = D3D11_QUERY_EVENT;
		m_device.getDevice()->CreateQuery( &desc, &m_waitIdleQuery );
	}
	
	Queue::~Queue()
	{
		safeRelease( m_waitIdleQuery );
	}

	void Queue::submit( ashes::CommandBufferCRefArray const & commandBuffers
		, ashes::SemaphoreCRefArray const & semaphoresToWait
		, ashes::VkPipelineStageFlagsArray const & semaphoresStage
		, ashes::SemaphoreCRefArray const & semaphoresToSignal
		, ashes::Fence const * fence )const
	{
		Context context{ m_device.getFeatureLevel(), m_device };

		for ( auto & commandBuffer : commandBuffers )
		{
			auto & dxCommandBuffer = static_cast< CommandBuffer const & >( commandBuffer.get() );
			dxCommandBuffer.execute( context );
			context.uavs.clear();
		}
	}

	ashes::VkResultArray Queue::present( ashes::SwapChainCRefArray const & swapChains
		, ashes::UInt32Array const & imagesIndex
		, ashes::SemaphoreCRefArray const & semaphoresToWait )const
	{
		ashes::VkResultArray result{ swapChains.size(), ashes::Result::eSuccess };

		for ( auto & swapChain : swapChains )
		{
			static_cast< SwapChain const & >( swapChain.get() ).getSwapChain()->Present( 0u, 0u );
		}

		return result;
	}

	void Queue::waitIdle()const
	{
		ID3D11DeviceContext * context;
		m_device.getDevice()->GetImmediateContext( &context );
		context->End( m_waitIdleQuery );
		context->Flush();
		BOOL data{ FALSE };

		while ( ( S_FALSE == context->GetData( m_waitIdleQuery
				, &data
				, UINT( sizeof( data ) )
				, 0u ) )
			&& !data )
		{
			std::this_thread::sleep_for( std::chrono::microseconds{ 1ull } );
		}

		safeRelease( context );
	}
}
