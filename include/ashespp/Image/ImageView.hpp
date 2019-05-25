/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ImageView_HPP___
#define ___AshesPP_ImageView_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

#include "ImageViewCreateInfo.hpp"

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
	struct ImageView
	{
		VkImageViewCreateInfo createInfo;
		VkImageView internal;
		Image const * image;

		ImageView( VkImageViewCreateInfo createInfo = {}
			, VkImageView internal = VK_NULL_HANDLE
			, Image const * image = nullptr );
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
			, uint32_t srcQueueFamily = ~( 0u )
			, uint32_t dstQueueFamily = ~( 0u ) )const;

		inline VkFormat getFormat()const
		{
			return createInfo.format;
		}
		/**
		*\~french
		*\brief
		*	Forward vers VkImageView.
		*\~english
		*\brief
		*	Forward to VkImageView.
		*/
		inline VkImageViewCreateInfo const * operator->()const
		{
			return &createInfo;
		}
		/**
		*\~french
		*\brief
		*	Conversion vers VkImageViewCreateInfo.
		*\~english
		*\brief
		*	VkImageViewCreateInfo cast operator.
		*/
		inline explicit operator VkImageViewCreateInfo const & ()const
		{
			return createInfo;
		}
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkImageView.
		*\~english
		*\brief
		*	VkImageView implicit cast operator.
		*/
		inline operator VkImageView const & ()const
		{
			return internal;
		}

	private:
		VkImageMemoryBarrier doMakeLayoutTransition( VkImageLayout srcLayout
			, VkImageLayout dstLayout
			, uint32_t srcQueueFamily
			, uint32_t dstQueueFamily )const;
		VkImageMemoryBarrier doMakeLayoutTransition( VkImageLayout srcLayout
			, VkImageLayout dstLayout
			, VkAccessFlags srcAccessFlags
			, uint32_t srcQueueFamily
			, uint32_t dstQueueFamily )const;
		VkImageMemoryBarrier doMakeLayoutTransition( VkImageLayout srcLayout
			, VkImageLayout dstLayout
			, VkAccessFlags srcAccessFlags
			, VkAccessFlags dstAccessMask
			, uint32_t srcQueueFamily
			, uint32_t dstQueueFamily )const;
	};
}

#endif
