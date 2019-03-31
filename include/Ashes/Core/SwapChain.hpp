/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_SwapChain_HPP___
#define ___Ashes_SwapChain_HPP___
#pragma once

#include "Ashes/Miscellaneous/SwapChainCreateInfo.hpp"

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
	protected:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\~french
		*\brief
		*	Constructeur.
		*/
		SwapChain( Device const & device
			, SwapChainCreateInfo createInfo );

	public:
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~SwapChain();
		/**
		*\~english
		*\return
		*	The images used by the swapchain.
		*\~french
		*\brief
		*	Les images utilisées par la swapchain.
		*/
		virtual ImagePtrArray getImages()const = 0;
		/**
		*\~french
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
		virtual Result acquireNextImage( uint64_t timeout
			, Semaphore const * semaphore
			, Fence const * fence
			, uint32_t & imageIndex )const = 0;
		/**
		*\~french
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
		Result acquireNextImage( uint64_t timeout
			, Semaphore const & semaphore
			, Fence const & fence
			, uint32_t & imageIndex )const;
		/**
		*\~french
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
		Result acquireNextImage( uint64_t timeout
			, Fence const & fence
			, uint32_t & imageIndex )const;
		/**
		*\~french
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
		Result acquireNextImage( uint64_t timeout
			, Semaphore const & semaphore
			, uint32_t & imageIndex )const;
		/**
		*\~french
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
		Result acquireNextImage( uint64_t timeout
			, uint32_t & imageIndex )const;
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
		
		inline Extent2D getDimensions()const
		{
			return m_createInfo.imageExtent;
		}

		inline PresentMode getPresentMode()const
		{
			return m_createInfo.presentMode;
		}

		inline Format getFormat()const
		{
			return m_createInfo.imageFormat;
		}
		/**@}*/

	protected:
		Device const & m_device;
		Surface const & m_surface;
		SwapChainCreateInfo m_createInfo;
	};
}

#endif
