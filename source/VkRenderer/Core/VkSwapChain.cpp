#include "Core/VkSwapChain.hpp"

#include "Core/VkDevice.hpp"
#include "Core/VkInstance.hpp"
#include "Core/VkPhysicalDevice.hpp"
#include "Core/VkSurface.hpp"
#include "Image/VkImage.hpp"
#include "Sync/VkFence.hpp"
#include "Sync/VkSemaphore.hpp"

#include <Ashes/Miscellaneous/MemoryRequirements.hpp>

namespace vk_renderer
{
	SwapChain::SwapChain( Device const & device
		, ashes::SwapChainCreateInfo createInfo )
		: ashes::SwapChain{ device, std::move( createInfo ) }
		, m_device{ device }
		, m_vkCreateInfo{ convert( ashes::SwapChain::m_createInfo ) }
	{
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateSwapchainKHR( m_device
			, &m_vkCreateInfo
			, nullptr
			, &m_swapChain );
		checkError( res, "Swap chain creation" );
	}

	SwapChain::~SwapChain()
	{
		m_device.vkDestroySwapchainKHR( m_device, m_swapChain, nullptr );
	}

	ashes::ImagePtrArray SwapChain::getImages()const
	{
		// On récupère les images de la swapchain.
		uint32_t imageCount{ 0u };
		auto res = m_device.vkGetSwapchainImagesKHR( m_device
			, m_swapChain
			, &imageCount
			, nullptr );
		checkError( res, "Swap chain images count retrieval" );

		std::vector< VkImage > swapChainImages( imageCount );
		res = m_device.vkGetSwapchainImagesKHR( m_device
			, m_swapChain
			, &imageCount
			, &swapChainImages[0] );
		checkError( res, "Swap chain images retrieval" );

		ashes::ImagePtrArray result;

		for ( auto image : swapChainImages )
		{
			result.emplace_back( std::make_unique< Image >( m_device
				, getFormat()
				, getDimensions()
				, image ) );
		}

		return result;
	}

	ashes::Result SwapChain::acquireNextImage( uint64_t timeout
		, ashes::Semaphore const * semaphore
		, ashes::Fence const * fence
		, uint32_t & imageIndex )const
	{
		return convert( m_device.vkAcquireNextImageKHR( m_device
			, m_swapChain
			, timeout
			, ( semaphore
				? VkSemaphore( static_cast< Semaphore const & >( *semaphore ) )
				: VK_NULL_HANDLE )
			, ( fence
				? VkFence( static_cast< Fence const & >( *fence ) )
				: VK_NULL_HANDLE )
			, &imageIndex ) );
	}
}
