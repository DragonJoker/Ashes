/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_MemoryBarrier_HPP___
#define ___Ashes_MemoryBarrier_HPP___
#pragma once

#include "AshesRenderer/AshesRendererPrerequisites.hpp"

namespace ashes
{
	/**
	*\brief
	*	Encapsulation d'un VkMemoryBarrier.
	*/
	struct MemoryBarrier
	{
		/**
		*\brief
		*	Constructeur.
		*\param[in] srcAccessMask
		*	Les indicateurs d'accès avant la barrière.
		*\param[in] dstAccessMask
		*	Les indicateurs d'accès après la barrière.
		*/
		MemoryBarrier( AccessFlags srcAccessMask
			, AccessFlags dstAccessMask )
			: srcAccessMask{ srcAccessMask }
			, dstAccessMask{ dstAccessMask }
		{
		}

		AccessFlags srcAccessMask;
		AccessFlags dstAccessMask;
	};
}

#endif
