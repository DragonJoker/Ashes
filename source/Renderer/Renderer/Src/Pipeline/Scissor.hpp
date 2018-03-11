/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_Scissor_HPP___
#define ___Renderer_Scissor_HPP___
#pragma once

#include "Miscellaneous/Extent2D.hpp"
#include "Miscellaneous/Offset2D.hpp"

namespace renderer
{
	/**
	*\brief
	*	Description d'un ciseau à utiliser lors de la création d'un pipeline ou à l'exécution.
	*/
	struct Scissor
	{
		/**
		*\brief
		*	Constructeur.
		*\param[in] x, y
		*	La position du ciseau.
		*\param[in] width, height
		*	Les dimensions du ciseau.
		*/
		Scissor( int32_t x
			, int32_t y
			, uint32_t width
			, uint32_t height );

		Offset2D offset;
		Extent2D size;
	};

	inline bool operator==( Scissor const & lhs, Scissor const & rhs )
	{
		return lhs.offset == rhs.offset
			&& lhs.size == rhs.size;
	}

	inline bool operator!=( Scissor const & lhs, Scissor const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif
