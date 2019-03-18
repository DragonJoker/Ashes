/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_SwapChain_HPP___
#define ___Ashes_SwapChain_HPP___
#pragma once

#include "Ashes/Core/BackBuffer.hpp"
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
		*	Creates the swap chain's depth and/or stencil image.
		*\param[in] format
		*	The pixel format.
		*\~french
		*\brief
		*	Crée l'image de profondeur et/ou stencil de la swap chain.
		*\param[in] format
		*	Le format de pixels.
		*/
		virtual void createDepthStencil( Format format ) = 0;
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

		inline BackBufferPtrArray const & getImages()const
		{
			return m_backBuffers;
		}

		inline PresentMode getPresentMode()const
		{
			return m_createInfo.presentMode;
		}

		inline TextureView const & getDepthStencilView()const
		{
			assert( m_depthStencilView );
			return *m_depthStencilView;
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
		BackBufferPtrArray m_backBuffers;
		mutable TexturePtr m_depthStencil;
		mutable TextureViewPtr m_depthStencilView;
	};
}

#endif
