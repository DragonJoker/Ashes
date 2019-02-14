/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ClipDirection_HPP___
#define ___Ashes_ClipDirection_HPP___
#pragma once

#include "Ashes/AshesPrerequisites.hpp"

namespace ashes
{
	/**
	*\~french
	*\brief
	*	Enumération des directions de clipping.
	*\~english
	*\brief
	*	Filters enumeration.
	*/
	enum class ClipDirection
	{
		eBottomUp,
		eTopDown,
	};
	/**
	*\~english
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*\~french
	*\brief
	*	Récupère le nom du type d'élément donné.
	*\param[in] value
	*	Le type d'élément.
	*\return
	*	Le nom.
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
