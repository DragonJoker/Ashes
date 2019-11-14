/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <ashespp/Enum/Format.hpp>

namespace utils
{
	/**
	*\brief
	*	Convertit un tampon d'un format donné dans un autre format.
	*\param[in] src
	*	Le tampon source.
	*\param[in] srcs
	*	La taille du tampon source
	*\param[in,out] dst
	*	Le tampon de destination (doit déjà avoir été alloué).
	*\param[in] dstf
	*	Le format voulu pour la destination.
	*\param[in] dsts
	*	La taille du tampon de destination.
	*/
	template< VkFormat Format >
	inline void convertBuffer( uint8_t const * src
		, size_t srcs
		, uint8_t * dst
		, VkFormat dstf
		, size_t dsts );
}

#include "Converter.inl"
