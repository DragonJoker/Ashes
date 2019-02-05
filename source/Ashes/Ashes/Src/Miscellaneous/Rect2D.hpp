/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_Rect2D_HPP___
#define ___Ashes_Rect2D_HPP___
#pragma once

#include "Miscellaneous/Offset2D.hpp"
#include "Miscellaneous/Extent2D.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies a two-dimensional subregion.
	*\~french
	*\brief
	*	Définit une sous-région en 2 dimensions.
	*/
	struct Rect2D
	{
		Offset2D offset;
		Extent2D extent;
	};

	inline bool operator==( Rect2D const & lhs, Rect2D const & rhs )
	{
		return lhs.offset == rhs.offset
			&& lhs.extent == rhs.extent;
	}

	inline bool operator!=( Rect2D const & lhs, Rect2D const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif
