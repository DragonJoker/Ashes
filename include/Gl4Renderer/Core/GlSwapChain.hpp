/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl4
{
	class SwapchainKHR
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	La connexion logique au GPU.
		*\param[in] createInfo
		*	Les informations de création.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] createInfo
		*	The creation informations.
		*/
		SwapchainKHR( VkDevice device
			, VkSwapchainCreateInfoKHR createInfo );
		~SwapchainKHR();
		uint32_t getImageCount()const;
		/**
		*\copydoc	SwapChain::getImages
		*/
		VkImageArray getImages()const;
		/**
		*\copydoc	SwapChain::acquireNextImage
		*/
		VkResult acquireNextImage( uint64_t timeout
			, VkSemaphore semaphore
			, VkFence fence
			, uint32_t & imageIndex )const;

		VkResult present( uint32_t imageIndex )const;

	private:
		VkDevice m_device;
		VkSwapchainCreateFlagsKHR m_flags;
		VkSurfaceKHR m_surface;
		uint32_t m_minImageCount;
		VkFormat m_imageFormat;
		VkColorSpaceKHR m_imageColorSpace;
		VkExtent2D m_imageExtent;
		uint32_t m_imageArrayLayers;
		VkImageUsageFlags m_imageUsage;
		VkSharingMode m_imageSharingMode;
		UInt32Array m_queueFamilyIndices;
		VkSurfaceTransformFlagBitsKHR m_preTransform;
		VkCompositeAlphaFlagBitsKHR m_compositeAlpha;
		VkPresentModeKHR m_presentMode;
		VkBool32 m_clipped;
		VkImage m_image;
		VkImageView m_view;
		GLuint m_fbo;
	};
}
