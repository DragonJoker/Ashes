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
			, uint32_t srcQueueFamily = VK_QUEUE_FAMILY_IGNORED
			, uint32_t dstQueueFamily = VK_QUEUE_FAMILY_IGNORED )const;
		/**
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
			, uint32_t srcQueueFamily = VK_QUEUE_FAMILY_IGNORED
			, uint32_t dstQueueFamily = VK_QUEUE_FAMILY_IGNORED )const;
		/**
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
			, uint32_t srcQueueFamily = VK_QUEUE_FAMILY_IGNORED
			, uint32_t dstQueueFamily = VK_QUEUE_FAMILY_IGNORED )const;
		/**
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
			, uint32_t srcQueueFamily = VK_QUEUE_FAMILY_IGNORED
			, uint32_t dstQueueFamily = VK_QUEUE_FAMILY_IGNORED )const;
		/**
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
			, uint32_t srcQueueFamily = VK_QUEUE_FAMILY_IGNORED
			, uint32_t dstQueueFamily = VK_QUEUE_FAMILY_IGNORED )const;
		/**
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
			, uint32_t srcQueueFamily = VK_QUEUE_FAMILY_IGNORED
			, uint32_t dstQueueFamily = VK_QUEUE_FAMILY_IGNORED )const;
		/**
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
			, uint32_t srcQueueFamily = VK_QUEUE_FAMILY_IGNORED
			, uint32_t dstQueueFamily = VK_QUEUE_FAMILY_IGNORED )const;
		/**
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
			, uint32_t srcQueueFamily = VK_QUEUE_FAMILY_IGNORED
			, uint32_t dstQueueFamily = VK_QUEUE_FAMILY_IGNORED )const;
		/**
		*\brief
		*	Prepares a transition memory barrier, to a specified layout.
		*\param[in] srcLayout
		*	The old layout.
		*\param[in] dstLayout
		*	The new layout.
		*\param[in] srcQueueFamily
		*	The source queue family, for a queue family transfer.
		*\param[in] dstQueueFamily
		*	The destination queue family, for a queue family transfer.
		*\return
		*	The memory barrier.
		*/
		VkImageMemoryBarrier makeLayoutTransition( VkImageLayout srcLayout
			, VkImageLayout dstLayout
			, uint32_t srcQueueFamily
			, uint32_t dstQueueFamily )const;
		/**
		*\brief
		*	Prepares a transition memory barrier, to a specified layout.
		*\param[in] srcLayout
		*	The old layout.
		*\param[in] dstLayout
		*	The new layout.
		*\param[in] srcAccessFlags
		*	The source access mask.
		*\param[in] srcQueueFamily
		*	The source queue family, for a queue family transfer.
		*\param[in] dstQueueFamily
		*	The destination queue family, for a queue family transfer.
		*\return
		*	The memory barrier.
		*/
		VkImageMemoryBarrier makeLayoutTransition( VkImageLayout srcLayout
			, VkImageLayout dstLayout
			, VkAccessFlags srcAccessFlags
			, uint32_t srcQueueFamily
			, uint32_t dstQueueFamily )const;
		/**
		*\brief
		*	Prepares a transition memory barrier, to a specified layout.
		*\param[in] srcLayout
		*	The old layout.
		*\param[in] dstLayout
		*	The new layout.
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
		VkImageMemoryBarrier makeLayoutTransition( VkImageLayout srcLayout
			, VkImageLayout dstLayout
			, VkAccessFlags srcAccessFlags
			, VkAccessFlags dstAccessMask
			, uint32_t srcQueueFamily
			, uint32_t dstQueueFamily )const;

		inline VkFormat getFormat()const
		{
			return createInfo.format;
		}
		/**
		*\brief
		*	Forward to VkImageView.
		*/
		inline VkImageViewCreateInfo const * operator->()const
		{
			return &createInfo;
		}
		/**
		*\brief
		*	Forward to VkImageView.
		*/
		inline VkImageViewCreateInfo * operator->()
		{
			return &createInfo;
		}
		/**
		*\brief
		*	VkImageViewCreateInfo cast operator.
		*/
		inline explicit operator VkImageViewCreateInfo const & ()const
		{
			return createInfo;
		}
		/**
		*\brief
		*	VkImageView implicit cast operator.
		*/
		inline operator VkImageView const & ()const
		{
			return internal;
		}
	};
}

#endif
