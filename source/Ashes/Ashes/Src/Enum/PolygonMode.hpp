/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_PolygonMode_HPP___
#define ___Ashes_PolygonMode_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Modes d'affichage des polygones.
	*/
	enum class PolygonMode
		: uint32_t
	{
		eFill,
		eLine,
		ePoint,
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
	inline std::string getName( PolygonMode value )
	{
		switch ( value )
		{
		case PolygonMode::eFill:
			return "fill";

		case PolygonMode::eLine:
			return "line";

		case PolygonMode::ePoint:
			return "point";

		default:
			assert( false && "Unsupported PolygonMode." );
			throw std::runtime_error{ "Unsupported PolygonMode" };
		}

		return 0;
	}
}

#endif
