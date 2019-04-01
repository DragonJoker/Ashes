/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/Core/SwapChain.hpp"

#include "AshesPP/Command/CommandBuffer.hpp"
#include "AshesPP/Core/Device.hpp"
#include "AshesPP/Image/Image.hpp"
#include "AshesPP/Sync/Fence.hpp"
#include "AshesPP/Sync/Semaphore.hpp"

namespace ashespp
{
	SwapChain::SwapChain( Device const & device
		, VkSwapchainCreateInfoKHR createInfo )
		: m_device{ device }
		, m_surface{ device.getSurface() }
		, m_createInfo{ std::move( createInfo ) }
	{
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateSwapchainKHR( m_device
			, &m_createInfo
			, nullptr
			, &m_internal );
		checkError( res, "Swap chain creation" );
		registerObject( m_device, "SwapChain", this );
	}

	SwapChain::~SwapChain()
	{
		unregisterObject( m_device, this );
		m_device.vkDestroySwapchainKHR( m_device, m_internal, nullptr );
	}

	VkResult SwapChain::acquireNextImage( uint64_t timeout
		, Semaphore const * semaphore
		, Fence const * fence
		, uint32_t & imageIndex )const
	{
		return m_device.vkAcquireNextImageKHR( m_device
			, m_internal
			, timeout
			, ( semaphore
				? VkSemaphore( static_cast< Semaphore const & >( *semaphore ) )
				: VK_NULL_HANDLE )
			, ( fence
				? VkFence( static_cast< Fence const & >( *fence ) )
				: VK_NULL_HANDLE )
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

	ImagePtrArray SwapChain::getImages()const
	{
		uint32_t imageCount{ 0u };
		auto res = m_device.vkGetSwapchainImagesKHR( m_device
			, m_internal
			, &imageCount
			, nullptr );
		checkError( res, "Swap chain images count retrieval" );

		std::vector< VkImage > swapChainImages( imageCount );
		res = m_device.vkGetSwapchainImagesKHR( m_device
			, m_internal
			, &imageCount
			, &swapChainImages[0] );
		checkError( res, "Swap chain images retrieval" );

		ImagePtrArray result;

		for ( auto & image : swapChainImages )
		{
			result.emplace_back( std::make_unique< Image >( m_device, image ) );
		}

		return result;
	}
}