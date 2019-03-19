#include "Core/VkSwapChain.hpp"

#include "Core/VkBackBuffer.hpp"
#include "Core/VkDevice.hpp"
#include "Core/VkInstance.hpp"
#include "Core/VkPhysicalDevice.hpp"
#include "Core/VkSurface.hpp"
#include "Image/VkTexture.hpp"
#include "Sync/VkFence.hpp"
#include "Sync/VkSemaphore.hpp"

#include <Ashes/Miscellaneous/MemoryRequirements.hpp>

namespace vk_renderer
{
	SwapChain::SwapChain( Device const & device
		, ashes::SwapChainCreateInfo createInfo )
		: ashes::SwapChain{ device, std::move( createInfo ) }
		, m_device{ device }
		, m_createInfo{ convert( ashes::SwapChain::m_createInfo ) }
	{
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateSwapchainKHR( m_device
			, &m_createInfo
			, nullptr
			, &m_swapChain );
		checkError( res, "Swap chain creation" );
		doCreateBackBuffers();
	}

	SwapChain::~SwapChain()
	{
		m_backBuffers.clear();
		m_device.vkDestroySwapchainKHR( m_device, m_swapChain, nullptr );
	}

	void SwapChain::createDepthStencil( ashes::Format format )
	{
		m_depthStencil = m_device.createTexture(
			{
				0u,
				ashes::TextureType::e2D,
				format,
				ashes::Extent3D{ getDimensions().width, getDimensions().height, 1u },
				1u,
				1u,
				ashes::SampleCountFlag::e1,
				ashes::ImageTiling::eOptimal,
				ashes::ImageUsageFlag::eDepthStencilAttachment,
				ashes::SharingMode::eExclusive,
				{},
				ashes::ImageLayout::eUndefined,
			} );
		m_depthStencil->bindMemory( m_device.allocateMemory( m_depthStencil->getMemoryRequirements()
			, ashes::MemoryPropertyFlag::eDeviceLocal ) );
		m_depthStencilView = m_depthStencil->createView( ashes::TextureViewType::e2D
			, format );
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

	void SwapChain::doCreateBackBuffers()
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

		// Et on crée des BackBuffers à partir de ces images.
		m_backBuffers.reserve( imageCount );
		uint32_t index{ 0u };

		for ( auto image : swapChainImages )
		{
			auto texture = std::make_unique< Texture >( m_device
				, getFormat()
				, getDimensions()
				, image );
			auto & ref = *texture;
			m_backBuffers.emplace_back( std::make_unique< BackBuffer >( m_device
				, std::move( texture )
				, index++
				, getFormat()
				, ref ) );
		}
	}
}
