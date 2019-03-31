/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_BufferMemoryBarrier_HPP___
#define ___Ashes_BufferMemoryBarrier_HPP___
#pragma once

#include "Ashes/Buffer/Buffer.hpp"

namespace ashes
{
	/**
	*\brief
	*	Encapsulation d'un VkBufferMemoryBarrier.
	*/
	struct BufferMemoryBarrier
	{
		/**
		*\brief
		*	Constructeur.
		*\param[in] srcAccessMask
		*	Les indicateurs d'accès avant la barrière.
		*\param[in] dstAccessMask
		*	Les indicateurs d'accès après la barrière.
		*\param[in] srcQueueFamilyIndex
		*	La famille de file voulue avant la barrière.
		*\param[in] dstQueueFamilyIndex
		*	La famille de file voulue après la barrière.
		*\param[in] buffer
		*	Le tampon.
		*\param[in] offset
		*	L'offset dans le tampon.
		*\param[in] size
		*	La taille du tampon.
		*/
		BufferMemoryBarrier( AccessFlags srcAccessMask
			, AccessFlags dstAccessMask
			, uint32_t srcQueueFamilyIndex
			, uint32_t dstQueueFamilyIndex
			, BufferBase const & buffer
			, uint64_t offset
			, uint64_t size )
			: srcAccessMask{ srcAccessMask }
			, dstAccessMask{ dstAccessMask }
			, srcQueueFamilyIndex{ srcQueueFamilyIndex }
			, dstQueueFamilyIndex{ dstQueueFamilyIndex }
			, buffer{ buffer }
			, offset{ offset }
			, size{ size }
		{
		}

		AccessFlags srcAccessMask;
		AccessFlags dstAccessMask;
		uint32_t srcQueueFamilyIndex;
		uint32_t dstQueueFamilyIndex;
		BufferBase const & buffer;
		uint64_t offset;
		uint64_t size;
	};
}

#endif
