/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_MemoryBarrier_HPP___
#define ___Ashes_MemoryBarrier_HPP___
#pragma once

#include "Ashes/AshesPrerequisites.hpp"

namespace ashes
{
	/**
	*\brief
	*	Encapsulation d'un VkMemoryBarrier.
	*/
	class MemoryBarrier
	{
	public:
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
			: m_srcAccessMask{ srcAccessMask }
			, m_dstAccessMask{ dstAccessMask }
		{
		}
		/**
		*\return
		*	Les indicateurs d'accès avant la barrière.
		*/
		inline AccessFlags getSrcAccessMask()const
		{
			return m_srcAccessMask;
		}
		/**
		*\return
		*	Les indicateurs d'accès après la barrière.
		*/
		inline AccessFlags getDstAccessMask()const
		{
			return m_dstAccessMask;
		}

	private:
		AccessFlags m_srcAccessMask;
		AccessFlags m_dstAccessMask;
	};
}

#endif
