/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ImageMemoryBarrier_HPP___
#define ___Ashes_ImageMemoryBarrier_HPP___
#pragma once

#include "Image/ImageSubresourceRange.hpp"

namespace ashes
{
	/**
	*\brief
	*	Encapsulation d'un vkImageMemoryBarrier.
	*/
	class ImageMemoryBarrier
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] srcAccessMask
		*	Les indicateurs d'accès avant la barrière.
		*\param[in] dstAccessMask
		*	Les indicateurs d'accès après la barrière.
		*\param[in] oldLayout
		*	Le layout d'image avant la barrière.
		*\param[in] newLayout
		*	Le layout d'image après la barrière.
		*\param[in] srcQueueFamilyIndex
		*	La famille de file voulue avant la barrière.
		*\param[in] dstQueueFamilyIndex
		*	La famille de file voulue après la barrière.
		*\param[in] image
		*	L'image.
		*\param[in] subresourceRange
		*	L'intervalle de sous-ressource.
		*/
		ImageMemoryBarrier( AccessFlags srcAccessMask
			, AccessFlags dstAccessMask
			, ImageLayout oldLayout
			, ImageLayout newLayout
			, uint32_t srcQueueFamilyIndex
			, uint32_t dstQueueFamilyIndex
			, Texture const & image
			, ImageSubresourceRange const & subresourceRange );

		AccessFlags srcAccessMask;
		AccessFlags dstAccessMask;
		ImageLayout oldLayout;
		ImageLayout newLayout;
		uint32_t srcQueueFamilyIndex;
		uint32_t dstQueueFamilyIndex;
		Texture const & image;
		ImageSubresourceRange subresourceRange;
	};
}

#endif
