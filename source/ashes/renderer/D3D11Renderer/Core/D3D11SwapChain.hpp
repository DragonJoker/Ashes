/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::D3D11_NAMESPACE
{
	class SwapchainKHR
		: public NonCopyable
	{
	public:
		SwapchainKHR( VkDevice device
			, VkSwapchainCreateInfoKHR createInfo );
		~SwapchainKHR()noexcept;

		uint32_t getImageCount()const;
		VkImageArray getImages()const;
		VkResult acquireNextImage( uint32_t & imageIndex )const;
		VkResult present()const;

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

	private:
		VkDevice m_device;
		VkSwapchainCreateInfoKHR m_createInfo;
#if defined( Ashes_D3D11_XBox )
		DXGI_SWAP_CHAIN_DESC1 m_presentDesc{};
#else
		DXGI_SWAP_CHAIN_DESC m_presentDesc;
#endif
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
