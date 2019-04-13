/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ImageView_HPP___
#define ___AshesPP_ImageView_HPP___
#pragma once

#include "AshesPP/AshesPPPrerequisites.hpp"

namespace ashes
{
	/**
	*\~french
	*\brief
	*	Description d'une vue sur une image.
	*\~english
	*\brief
	*	Image view wrapper.
	*/
	class ImageView
	{
	public:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	La connexion logique au GPU.
		*\param[in] image
		*	L'image sur laquelle la vue est créée.
		*\param[in] createInfo
		*	Les informations de création de la vue.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] image
		*	The image from which the view is created.
		*\param[in] createInfo
		*	The view creation informations.
		*/
		ImageView( Device const & device
			, Image const & image
			, VkImageViewCreateInfo const & createInfo );
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	La connexion logique au GPU.
		*\param[in] createInfo
		*	Les informations de création de la vue.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical connection to the GPU.
		*\param[in] createInfo
		*	The view creation informations.
		*/
		ImageView( Device const & device
			, VkImageViewCreateInfo const & createInfo );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~ImageView();
		/**
		*\~french
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout général.
		*\param[in] srcLayout
		*	Le layout source.
		*\param[in] srcAccessFlags
		*	Les accès source.
		*\param[in] dstAccessFlags
		*	Les accès destination.
		*\param[in] srcQueueFamily
		*	La famille de file source, pour un transfer de famille de file.
		*\param[in] dstQueueFamily
		*	La famille de file destination, pour un transfer de famille de file.
		*\return
		*	La barrière mémoire.
		*\~english
		*\brief
		*	Prepares a transition memory barrier, for a general layout.
		*\param[in] srcLayout
		*	The old layout.
		*\param[in] srcAccessFlags
		*	The source access mask.
		*\param[in] dstAccessFlags
		*	The destination access mask.
		*\param[in] srcQueueFamily
		*	The source queue family, for a queue family transfer.
		*\param[in] dstQueueFamily
		*	The destination queue family, for a queue family transfer.
		*\return
		*	The memory barrier.
		*/
		VkImageMemoryBarrier makeGeneralLayout( VkImageLayout srcLayout
			, VkAccessFlags srcAccessFlags
			, VkAccessFlags dstAccessFlags
			, uint32_t srcQueueFamily = ~( 0u )
			, uint32_t dstQueueFamily = ~( 0u ) )const;
		/**
		*\~french
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de destination de transfert.
		*\param[in] srcLayout
		*	Le layout source.
		*\param[in] srcAccessFlags
		*	Les accès source.
		*\param[in] srcQueueFamily
		*	La famille de file source, pour un transfer de famille de file.
		*\param[in] dstQueueFamily
		*	La famille de file destination, pour un transfer de famille de file.
		*\return
		*	La barrière mémoire.
		*\~english
		*\brief
		*	Prepares a transition memory barrier, for a transfer destination layout.
		*\param[in] srcLayout
		*	The old layout.
		*\param[in] srcAccessFlags
		*	The source access mask.
		*\param[in] srcQueueFamily
		*	The source queue family, for a queue family transfer.
		*\param[in] dstQueueFamily
		*	The destination queue family, for a queue family transfer.
		*\return
		*	The memory barrier.
		*/
		VkImageMemoryBarrier makeTransferDestination( VkImageLayout srcLayout
			, VkAccessFlags srcAccessFlags
			, uint32_t srcQueueFamily = ~( 0u )
			, uint32_t dstQueueFamily = ~( 0u ) )const;
		/**
		*\~french
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de source de transfert.
		*\param[in] srcLayout
		*	Le layout source.
		*\param[in] srcAccessFlags
		*	Les accès source.
		*\param[in] srcQueueFamily
		*	La famille de file source, pour un transfer de famille de file.
		*\param[in] dstQueueFamily
		*	La famille de file destination, pour un transfer de famille de file.
		*\return
		*	La barrière mémoire.
		*\~english
		*\brief
		*	Prepares a transition memory barrier, for a transfer source layout.
		*\param[in] srcLayout
		*	The old layout.
		*\param[in] srcAccessFlags
		*	The source access mask.
		*\param[in] srcQueueFamily
		*	The source queue family, for a queue family transfer.
		*\param[in] dstQueueFamily
		*	The destination queue family, for a queue family transfer.
		*\return
		*	The memory barrier.
		*/
		VkImageMemoryBarrier makeTransferSource( VkImageLayout srcLayout
			, VkAccessFlags srcAccessFlags
			, uint32_t srcQueueFamily = ~( 0u )
			, uint32_t dstQueueFamily = ~( 0u ) )const;
		/**
		*\~french
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de ressource d'entrée (lecture seule) d'un shader.
		*\param[in] srcLayout
		*	Le layout source.
		*\param[in] srcAccessFlags
		*	Les accès source.
		*\param[in] srcQueueFamily
		*	La famille de file source, pour un transfer de famille de file.
		*\param[in] dstQueueFamily
		*	La famille de file destination, pour un transfer de famille de file.
		*\return
		*	La barrière mémoire.
		*\~english
		*\brief
		*	Prepares a transition memory barrier, for an input shader resource layout.
		*\param[in] srcLayout
		*	The old layout.
		*\param[in] srcAccessFlags
		*	The source access mask.
		*\param[in] srcQueueFamily
		*	The source queue family, for a queue family transfer.
		*\param[in] dstQueueFamily
		*	The destination queue family, for a queue family transfer.
		*\return
		*	The memory barrier.
		*/
		VkImageMemoryBarrier makeShaderInputResource( VkImageLayout srcLayout
			, VkAccessFlags srcAccessFlags
			, uint32_t srcQueueFamily = ~( 0u )
			, uint32_t dstQueueFamily = ~( 0u ) )const;
		/**
		*\~french
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de ressource de lecture de profondeur et/ou stencil.
		*\param[in] srcLayout
		*	Le layout source.
		*\param[in] srcAccessFlags
		*	Les accès source.
		*\param[in] srcQueueFamily
		*	La famille de file source, pour un transfer de famille de file.
		*\param[in] dstQueueFamily
		*	La famille de file destination, pour un transfer de famille de file.
		*\return
		*	La barrière mémoire.
		*\~english
		*\brief
		*	Prepares a transition memory barrier, for a depth ant/or stencil read only layout.
		*\param[in] srcLayout
		*	The old layout.
		*\param[in] srcAccessFlags
		*	The source access mask.
		*\param[in] srcQueueFamily
		*	The source queue family, for a queue family transfer.
		*\param[in] dstQueueFamily
		*	The destination queue family, for a queue family transfer.
		*\return
		*	The memory barrier.
		*/
		VkImageMemoryBarrier makeDepthStencilReadOnly( VkImageLayout srcLayout
			, VkAccessFlags srcAccessFlags
			, uint32_t srcQueueFamily = ~( 0u )
			, uint32_t dstQueueFamily = ~( 0u ) )const;
		/**
		*\~french
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout d'attache couleur.
		*\param[in] srcLayout
		*	Le layout source.
		*\param[in] srcAccessFlags
		*	Les accès source.
		*\param[in] srcQueueFamily
		*	La famille de file source, pour un transfer de famille de file.
		*\param[in] dstQueueFamily
		*	La famille de file destination, pour un transfer de famille de file.
		*\return
		*	La barrière mémoire.
		*\~english
		*\brief
		*	Prepares a transition memory barrier, for a colour attachment layout.
		*\param[in] srcLayout
		*	The old layout.
		*\param[in] srcAccessFlags
		*	The source access mask.
		*\param[in] srcQueueFamily
		*	The source queue family, for a queue family transfer.
		*\param[in] dstQueueFamily
		*	The destination queue family, for a queue family transfer.
		*\return
		*	The memory barrier.
		*/
		VkImageMemoryBarrier makeColourAttachment( VkImageLayout srcLayout
			, VkAccessFlags srcAccessFlags
			, uint32_t srcQueueFamily = ~( 0u )
			, uint32_t dstQueueFamily = ~( 0u ) )const;
		/**
		*\~french
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout d'attache profondeur et/ou stencil.
		*\param[in] srcLayout
		*	Le layout source.
		*\param[in] srcAccessFlags
		*	Les accès source.
		*\param[in] srcQueueFamily
		*	La famille de file source, pour un transfer de famille de file.
		*\param[in] dstQueueFamily
		*	La famille de file destination, pour un transfer de famille de file.
		*\return
		*	La barrière mémoire.
		*\~english
		*\brief
		*	Prepares a transition memory barrier, for a depth and/or stencil attachment layout.
		*\param[in] srcLayout
		*	The old layout.
		*\param[in] srcAccessFlags
		*	The source access mask.
		*\param[in] srcQueueFamily
		*	The source queue family, for a queue family transfer.
		*\param[in] dstQueueFamily
		*	The destination queue family, for a queue family transfer.
		*\return
		*	The memory barrier.
		*/
		VkImageMemoryBarrier makeDepthStencilAttachment( VkImageLayout srcLayout
			, VkAccessFlags srcAccessFlags
			, uint32_t srcQueueFamily = ~( 0u )
			, uint32_t dstQueueFamily = ~( 0u ) )const;
		/**
		*\~french
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de source de presentation.
		*\param[in] srcLayout
		*	Le layout source.
		*\param[in] srcAccessFlags
		*	Les accès source.
		*\param[in] srcQueueFamily
		*	La famille de file source, pour un transfer de famille de file.
		*\param[in] dstQueueFamily
		*	La famille de file destination, pour un transfer de famille de file.
		*\return
		*	La barrière mémoire.
		*\~english
		*\brief
		*	Prepares a transition memory barrier, for a presentation source layout.
		*\param[in] srcLayout
		*	The old layout.
		*\param[in] srcAccessFlags
		*	The source access mask.
		*\param[in] srcQueueFamily
		*	The source queue family, for a queue family transfer.
		*\param[in] dstQueueFamily
		*	The destination queue family, for a queue family transfer.
		*\return
		*	The memory barrier.
		*/
		VkImageMemoryBarrier makePresentSource( VkImageLayout srcLayout
			, VkAccessFlags srcAccessFlags
			, uint32_t srcQueueFamily = ~( 0u )
			, uint32_t dstQueueFamily = ~( 0u ) )const;
		/**
		*\~english
		*name
		*	Getters.
		*\~french
		*name
		*	Accesseurs.
		*/
		/**@{*/
		inline VkImageViewType getType()const
		{
			return m_createInfo.viewType;
		}

		inline VkFormat getFormat()const
		{
			return m_createInfo.format;
		}

		inline Image const & getImage()const
		{
			assert( m_image );
			return *m_image;
		}

		inline VkComponentMapping const & getComponentMapping()const
		{
			return m_createInfo.components;
		}

		inline VkImageSubresourceRange const & getSubResourceRange()const
		{
			return m_createInfo.subresourceRange;
		}
		/**@}*/
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkImageView.
		*\~english
		*\brief
		*	VkImageView implicit cast operator.
		*/
		inline operator VkImageView const & ( )const
		{
			return m_internal;
		}

	private:
		VkImageMemoryBarrier doMakeLayoutTransition( VkImageLayout srcLayout
			, VkImageLayout dstLayout
			, VkAccessFlags srcAccessFlags
			, VkAccessFlags dstAccessMask
			, uint32_t srcQueueFamily
			, uint32_t dstQueueFamily )const;

	protected:
		VkImageViewCreateInfo m_createInfo;

	private:
		Device const & m_device;
		Image const * m_image;
		VkImageView m_internal{ VK_NULL_HANDLE };
	};
}

#endif
