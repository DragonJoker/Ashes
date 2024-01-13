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
	
	Queue::~Queue()noexcept
	{
		safeRelease( m_waitIdleQuery );
	}

	VkResult Queue::submit( ArrayView< VkSubmitInfo const > const & infos )const
	{
		for ( auto & info : infos )
		{
			doSubmit( makeArrayView( info.pCommandBuffers, info.commandBufferCount ) );
		}

		return VK_SUCCESS;
	}

	VkResult Queue::bindSparse()const
	{
		return VK_ERROR_FEATURE_NOT_PRESENT;
	}

	VkResult Queue::present( VkPresentInfoKHR const & presentInfo )const
	{
		auto itIndices = presentInfo.pImageIndices;

		if ( presentInfo.pResults )
		{
			auto itResult = presentInfo.pResults;

			for ( auto & swapChain : makeArrayView( presentInfo.pSwapchains, presentInfo.swapchainCount ) )
			{
				*itResult = get( swapChain )->present();
				++itResult;
				++itIndices;
			}
		}
		else
		{
			for ( auto & swapChain : makeArrayView( presentInfo.pSwapchains, presentInfo.swapchainCount ) )
			{
				get( swapChain )->present();
				++itIndices;
			}
		}

		return VK_SUCCESS;
	}

	VkResult Queue::waitIdle()const
	{
		auto context{ get( m_device )->getImmediateContext() };
		context->End( m_waitIdleQuery );
		context->Flush();
		BOOL data{ FALSE };

		while ( ( S_FALSE == context->GetData( m_waitIdleQuery
				, &data
				, UINT( sizeof( data ) )
				, 0u ) )
			&& !data )
		{
			std::this_thread::sleep_for( std::chrono::microseconds{ 1ULL } );
		}

		return data
			? VK_SUCCESS
			: VK_TIMEOUT;
	}

#if VK_EXT_debug_utils

	void Queue::beginDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const
	{
		m_label = DebugLabel
		{
			{ labelInfo.color[0], labelInfo.color[1], labelInfo.color[2], labelInfo.color[3] },
			labelInfo.pLabelName,
		};
	}

	void Queue::endDebugUtilsLabel()const
	{
		m_label = ashes::nullopt;
	}

	void Queue::insertDebugUtilsLabel( VkDebugUtilsLabelEXT const & labelInfo )const
	{
		m_label = DebugLabel
		{
			{ labelInfo.color[0], labelInfo.color[1], labelInfo.color[2], labelInfo.color[3] },
			labelInfo.pLabelName,
		};
	}

#endif
#if VK_EXT_debug_marker

	void Queue::debugMarkerBegin( VkDebugMarkerMarkerInfoEXT const & labelInfo )const
	{
		m_label = DebugLabel
		{
			{ labelInfo.color[0], labelInfo.color[1], labelInfo.color[2], labelInfo.color[3] },
			labelInfo.pMarkerName,
		};
	}

	void Queue::debugMarkerEnd()const
	{
		m_label = ashes::nullopt;
	}

	void Queue::debugMarkerInsert( VkDebugMarkerMarkerInfoEXT const & labelInfo )const
	{
		m_label = DebugLabel
		{
			{ labelInfo.color[0], labelInfo.color[1], labelInfo.color[2], labelInfo.color[3] },
			labelInfo.pMarkerName,
		};
	}

#endif

	VkResult Queue::doSubmit( ArrayView< VkCommandBuffer const > const & commandBuffers )const
	{
		Context context{ get( m_device )->getFeatureLevel(), m_device };

		for ( auto const & commandBuffer : commandBuffers )
		{
			auto const & dxCommandBuffer = *get( commandBuffer );
			dxCommandBuffer.execute( context );
			context.uavs.clear();
		}

		return VK_SUCCESS;
	}
}
