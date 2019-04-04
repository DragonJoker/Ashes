/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_SwapChain_HPP___
#define ___AshesPP_SwapChain_HPP___
#pragma once

#include "AshesPP/AshesPPPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Class holding the concept of a swapchain.
	*\remarks
	*	Provides the ability to present rendering results to a surface.
	*\~french
	*\brief
	*	Classe gérant le concept de swapchain.
	*\remarks
	*	Fournit la possibilité de présenter le résultat d'un rendu à une surface.
	*/
	class SwapChain
	{
	public:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\~french
		*\brief
		*	Constructeur.
		*/
		SwapChain( Device const & device
			, VkSwapchainCreateInfoKHR createInfo );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~SwapChain();
		/**
		*\~english
		*\brief
		*	Acquires an available presentable image to use, and retrieve the index of that image.
		*\param[in] timeout
		*	Specifies how long the function waits, in nanoseconds, if no image is available.
		*\param[in] semaphore
		*	\p nullptr or a semaphore to signal.
		*\param[in] fence
		*	\p nullptr or a fence to signal.
		*\param[out] imageIndex
		*	Set to the index of the next image to use.
		*\~french
		*\brief
		*	Acquiert une image présentable disponible, et récupère l'indice de cette image.
		*\param[in] timeout
		*	Définit combien de temps la fonction attend, en nanosecondes, si aucune image n'est disponible.
		*\param[in] semaphore
		*	\p nullptr ou un sémaphore à signaler.
		*\param[in] fence
		*	\p nullptr ou une fence à signaler.
		*\param[out] imageIndex
		*	Défini à la prochaine image à utiliser.
		*/
		VkResult acquireNextImage( uint64_t timeout
			, Semaphore const * semaphore
			, Fence const * fence
			, uint32_t & imageIndex )const;
		/**
		*\~english
		*\brief
		*	Acquires an available presentable image to use, and retrieve the index of that image.
		*\param[in] timeout
		*	Specifies how long the function waits, in nanoseconds, if no image is available.
		*\param[in] semaphore
		*	A semaphore to signal.
		*\param[in] fence
		*	A fence to signal.
		*\param[out] imageIndex
		*	Set to the index of the next image to use.
		*\~french
		*\brief
		*	Acquiert une image présentable disponible, et récupère l'indice de cette image.
		*\param[in] timeout
		*	Définit combien de temps la fonction attend, en nanosecondes, si aucune image n'est disponible.
		*\param[in] semaphore
		*	Un sémaphore à signaler.
		*\param[in] fence
		*	Une fence à signaler.
		*\param[out] imageIndex
		*	Défini à la prochaine image à utiliser.
		*/
		VkResult acquireNextImage( uint64_t timeout
			, Semaphore const & semaphore
			, Fence const & fence
			, uint32_t & imageIndex )const;
		/**
		*\~english
		*\brief
		*	Acquires an available presentable image to use, and retrieve the index of that image.
		*\param[in] timeout
		*	Specifies how long the function waits, in nanoseconds, if no image is available.
		*\param[in] fence
		*	A fence to signal.
		*\param[out] imageIndex
		*	Set to the index of the next image to use.
		*\~french
		*\brief
		*	Acquiert une image présentable disponible, et récupère l'indice de cette image.
		*\param[in] timeout
		*	Définit combien de temps la fonction attend, en nanosecondes, si aucune image n'est disponible.
		*\param[in] fence
		*	Une fence à signaler.
		*\param[out] imageIndex
		*	Défini à la prochaine image à utiliser.
		*/
		VkResult acquireNextImage( uint64_t timeout
			, Fence const & fence
			, uint32_t & imageIndex )const;
		/**
		*\~english
		*\brief
		*	Acquires an available presentable image to use, and retrieve the index of that image.
		*\param[in] timeout
		*	Specifies how long the function waits, in nanoseconds, if no image is available.
		*\param[in] semaphore
		*	\p nullptr or a semaphore to signal.
		*\param[out] imageIndex
		*	Set to the index of the next image to use.
		*\~french
		*\brief
		*	Acquiert une image présentable disponible, et récupère l'indice de cette image.
		*\param[in] timeout
		*	Définit combien de temps la fonction attend, en nanosecondes, si aucune image n'est disponible.
		*\param[in] semaphore
		*	\p nullptr ou un sémaphore à signaler.
		*\param[out] imageIndex
		*	Défini à la prochaine image à utiliser.
		*/
		VkResult acquireNextImage( uint64_t timeout
			, Semaphore const & semaphore
			, uint32_t & imageIndex )const;
		/**
		*\~english
		*\brief
		*	Acquires an available presentable image to use, and retrieve the index of that image.
		*\param[in] timeout
		*	Specifies how long the function waits, in nanoseconds, if no image is available.
		*\param[out] imageIndex
		*	Set to the index of the next image to use.
		*\~french
		*\brief
		*	Acquiert une image présentable disponible, et récupère l'indice de cette image.
		*\param[in] timeout
		*	Définit combien de temps la fonction attend, en nanosecondes, si aucune image n'est disponible.
		*\param[out] imageIndex
		*	Défini à la prochaine image à utiliser.
		*/
		VkResult acquireNextImage( uint64_t timeout
			, uint32_t & imageIndex )const;
		/**
		*\~english
		*\return
		*	The images used by the swapchain.
		*\~french
		*\brief
		*	Les images utilisées par la swapchain.
		*/
		ImagePtrArray getImages()const;
		/**
		*\~english
		*name
		*	Getters.
		*\~french
		*name
		*	Accesseurs.
		*/
		/**@{*/
		inline Device const & getDevice()const
		{
			return m_device;
		}

		inline uint32_t getImageCount()const
		{
			return m_createInfo.minImageCount;
		}

		inline VkExtent2D getDimensions()const
		{
			return m_createInfo.imageExtent;
		}

		inline VkPresentModeKHR getPresentMode()const
		{
			return m_createInfo.presentMode;
		}

		inline VkFormat getFormat()const
		{
			return m_createInfo.imageFormat;
		}
		/**@}*/
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkSwapchainKHR.
		*\~english
		*\brief
		*	VkSwapchainKHR implicit cast operator.
		*/
		inline operator VkSwapchainKHR const & ()const
		{
			return m_internal;
		}

	protected:
		Device const & m_device;
		Surface const & m_surface;
		VkSwapchainCreateInfoKHR m_createInfo;
		VkSwapchainKHR m_internal{ VK_NULL_HANDLE };
	};
}

#endif
