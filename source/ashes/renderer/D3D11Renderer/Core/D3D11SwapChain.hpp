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

		inline IDXGISwapChain * getSwapChain()const
		{
			return m_swapChain;
		}

	private:
		DXGI_SWAP_CHAIN_DESC doInitPresentParameters();

	protected:
		VkDevice m_device;
		VkSwapchainCreateInfoKHR m_createInfo;
		uint32_t m_currentBuffer{};
		IDXGISwapChain * m_swapChain;
	};
}
