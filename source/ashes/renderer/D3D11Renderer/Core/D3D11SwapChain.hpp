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

		VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		void doInitPresentParameters();

	protected:
		VkDevice m_device;
		VkSwapchainCreateInfoKHR m_createInfo;
		DXGI_SWAP_CHAIN_DESC m_presentDesc;
		DXGI_MODE_DESC m_displayMode;
		uint32_t m_currentBuffer{};
		IDXGISwapChain * m_swapChain{ nullptr };
		VkImage m_swapChainImage{};
		VkImage m_image{};
		VkDeviceMemory m_deviceMemory{};
		VkImageView m_view{};
		VkExtent2D m_swapchainExtent;
		VkExtent2D m_windowExtent;
	};
}
