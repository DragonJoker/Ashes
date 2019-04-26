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

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	Queue::Queue( VkDevice device
		, VkDeviceQueueCreateInfo createInfo
		, uint32_t index )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
		, m_index{ index }
	{
		D3D11_QUERY_DESC desc{};
		desc.Query = D3D11_QUERY_EVENT;
		get( m_device )->getDevice()->CreateQuery( &desc, &m_waitIdleQuery );
	}
	
	Queue::~Queue()
	{
		safeRelease( m_waitIdleQuery );
	}

	VkResult Queue::submit( VkSubmitInfoArray const & infos
		, VkFence fence )const
	{
		for ( auto & info : infos )
		{
			doSubmit( { info.pCommandBuffers, info.pCommandBuffers + info.commandBufferCount }
				, { info.pWaitSemaphores, info.pWaitSemaphores + info.waitSemaphoreCount }
				, { info.pWaitDstStageMask, info.pWaitDstStageMask + info.waitSemaphoreCount }
				, { info.pSignalSemaphores, info.pSignalSemaphores + info.signalSemaphoreCount }
				, fence );
		}

		return VK_SUCCESS;
	}

	VkResult Queue::present( VkPresentInfoKHR const & presentInfo )const
	{
		if ( presentInfo.pResults )
		{
			auto itResult = presentInfo.pResults;

			for ( auto & swapChain : makeArrayView( presentInfo.pSwapchains, presentInfo.swapchainCount ) )
			{
				*itResult = checkError( m_device, get( swapChain )->getSwapChain()->Present( 0u, 0u ), "Presentation" )
					? VK_SUCCESS
					: VK_ERROR_SURFACE_LOST_KHR;
				++itResult;
			}
		}
		else
		{
			for ( auto & swapChain : makeArrayView( presentInfo.pSwapchains, presentInfo.swapchainCount ) )
			{
				get( swapChain )->getSwapChain()->Present( 0u, 0u );
			}
		}

		return VK_SUCCESS;
	}

	VkResult Queue::waitIdle()const
	{
		ID3D11DeviceContext * context;
		get( m_device )->getDevice()->GetImmediateContext( &context );
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
		return data
			? VK_SUCCESS
			: VK_TIMEOUT;
	}

	VkResult Queue::doSubmit( VkCommandBufferArray const & commandBuffers
		, VkSemaphoreArray const & semaphoresToWait
		, VkPipelineStageFlagsArray const & semaphoresStage
		, VkSemaphoreArray const & semaphoresToSignal
		, VkFence fence )const
	{
		Context context{ get( m_device )->getFeatureLevel(), m_device };

		for ( auto & commandBuffer : commandBuffers )
		{
			auto & dxCommandBuffer = *get( commandBuffer );
			dxCommandBuffer.execute( context );
			context.uavs.clear();
		}

		return VK_SUCCESS;
	}
}
