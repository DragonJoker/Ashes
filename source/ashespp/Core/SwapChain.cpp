/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Core/SwapChain.hpp"

#include "ashespp/Command/CommandBuffer.hpp"
#include "ashespp/Core/Device.hpp"
#include "ashespp/Image/Image.hpp"
#include "ashespp/Sync/Fence.hpp"
#include "ashespp/Sync/Semaphore.hpp"

namespace ashes
{
	SwapChain::SwapChain( Device const & device
		, VkSwapchainCreateInfoKHR createInfo )
		: SwapChain{ device, "SwapChain", std::move( createInfo ) }
	{
	}
	
	SwapChain::SwapChain( Device const & device
		, std::string const & debugName
		, VkSwapchainCreateInfoKHR createInfo )
		: VkObject{ debugName }
		, m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateSwapchainKHR( m_device
			, &m_createInfo
			, m_device.getAllocationCallbacks()
			, &m_internal );
		checkError( res, "Swap chain creation" );
		registerObject( m_device, debugName, *this );

		uint32_t imageCount{ 0u };
		res = m_device.vkGetSwapchainImagesKHR( m_device
			, m_internal
			, &imageCount
			, nullptr );
		checkError( res, "Swap chain images count retrieval" );

		if ( imageCount )
		{
			VkImageArray images;
			images.resize( imageCount );
			res = m_device.vkGetSwapchainImagesKHR( m_device
				, m_internal
				, &imageCount
				, &images[0] );
			checkError( res, "Swap chain images retrieval" );

			for ( auto image : images )
			{
				m_images.emplace_back( std::make_unique< Image >( m_device, image ) );
			}
		}
	}

	SwapChain::~SwapChain()noexcept
	{
		unregisterObject( m_device, *this );
		m_device.vkDestroySwapchainKHR( m_device
			, m_internal
			, m_device.getAllocationCallbacks() );
	}

	VkResult SwapChain::acquireNextImage( uint64_t timeout
		, Semaphore const * semaphore
		, Fence const * fence
		, uint32_t & imageIndex )const
	{
		if ( semaphore )
		{
			semaphore->signal( fence );
		}

		return m_device.vkAcquireNextImageKHR( m_device
			, m_internal
			, timeout
			, ( semaphore
				? VkSemaphore( *semaphore )
				: VkSemaphore{} )
			, ( fence
				? VkFence( *fence )
				: VkFence{} )
			, &imageIndex );
	}

	VkResult SwapChain::acquireNextImage( uint64_t timeout
		, Semaphore const & semaphore
		, Fence const & fence
		, uint32_t & imageIndex )const
	{
		return acquireNextImage( timeout
			, &semaphore
			, &fence
			, imageIndex );
	}

	VkResult SwapChain::acquireNextImage( uint64_t timeout
		, Fence const & fence
		, uint32_t & imageIndex )const
	{
		return acquireNextImage( timeout
			, nullptr
			, &fence
			, imageIndex );
	}

	VkResult SwapChain::acquireNextImage( uint64_t timeout
		, Semaphore const & semaphore
		, uint32_t & imageIndex )const
	{
		return acquireNextImage( timeout
			, &semaphore
			, nullptr
			, imageIndex );
	}

	VkResult SwapChain::acquireNextImage( uint64_t timeout
		, uint32_t & imageIndex )const
	{
		return acquireNextImage( timeout
			, nullptr
			, nullptr
			, imageIndex );
	}
}
