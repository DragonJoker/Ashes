/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	class SwapchainKHR
	{
	public:
		SwapchainKHR( VkDevice device
			, VkSwapchainCreateInfoKHR createInfo );
		~SwapchainKHR();

		uint32_t getImageCount()const;
		VkImageArray getImages()const;
		VkResult acquireNextImage( uint64_t timeout
			, VkSemaphore semaphore
			, VkFence fence
			, uint32_t & imageIndex )const;
		VkResult present( uint32_t imageIndex )const;

		inline IDXGISwapChain * getSwapChain()const
		{
			return m_swapChain;
		}

	private:
		void doInitPresentParameters();

	protected:
		VkDevice m_device;
		VkSwapchainCreateInfoKHR m_createInfo;
		DXGI_SWAP_CHAIN_DESC m_presentDesc;
		uint32_t m_currentBuffer{};
		IDXGISwapChain * m_swapChain{ nullptr };
		VkImage m_swapChainImage{ VK_NULL_HANDLE };
		VkImage m_image{ VK_NULL_HANDLE };
		VkDeviceMemory m_deviceMemory{ VK_NULL_HANDLE };
		VkImageView m_view{ VK_NULL_HANDLE };
	};
}
