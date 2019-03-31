/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "VkRenderer/VkRendererPrerequisites.hpp"

#include <Ashes/Core/SwapChain.hpp>

namespace vk_renderer
{
	class SwapChain
		: public ashes::SwapChain
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	La connexion logique au GPU.
		*\param[in] createInfo
		*	Les informations de cr�ation.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] createInfo
		*	The creation informations.
		*/
		SwapChain( Device const & device
			, ashes::SwapChainCreateInfo createInfo );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~SwapChain();
		/**
		*\copydoc	ashes::SwapChain::getImages
		*/
		ashes::ImagePtrArray getImages()const override;
		/**
		*\copydoc	ashes::SwapChain::acquireNextImage
		*/
		ashes::Result acquireNextImage( uint64_t timeout
			, ashes::Semaphore const * semaphore
			, ashes::Fence const * fence
			, uint32_t & imageIndex )const override;
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkSwapchainKHR.
		*\~english
		*\brief
		*	VkSwapchainKHR implicit cast operator.
		*/
		inline operator VkSwapchainKHR const &()const
		{
			return m_swapChain;
		}

	protected:
		Device const & m_device;
		VkSwapchainKHR m_swapChain{};
		VkSwapchainCreateInfoKHR m_vkCreateInfo;
	};
}
