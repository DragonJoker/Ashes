/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_Offset2D_HPP___
#define ___Ashes_Offset2D_HPP___
#pragma once

#include "AshesPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies a two dimensional offset.
	*\~french
	*\brief
	*	Définit un décalage en 2 dimensions.
	*/
	struct Offset2D
	{
		int32_t x;
		int32_t y;
	};

	inline bool operator==( Offset2D const & lhs, Offset2D const & rhs )
	{
		return lhs.x == rhs.x
			&& lhs.y == rhs.y;
	}

	inline bool operator!=( Offset2D const & lhs, Offset2D const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif
