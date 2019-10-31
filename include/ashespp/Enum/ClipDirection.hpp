/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ClipDirection_HPP___
#define ___AshesPP_ClipDirection_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	/**
	*\brief
	*	Filters enumeration.
	*/
	enum class ClipDirection
	{
		eBottomUp,
		eTopDown,
	};
	/**
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*/
	inline std::string getName( ClipDirection value )
	{
		switch ( value )
		{
		case ClipDirection::eBottomUp:
			return "bottom_up";
		case ClipDirection::eTopDown:
			return "top_down";
		default:
			assert( false && "Unsupported ClipDirection." );
			throw std::runtime_error{ "Unsupported ClipDirection" };
		}

		return 0;
	}
}

#endif
