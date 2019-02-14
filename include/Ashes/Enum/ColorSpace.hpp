/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ColorSpace_HPP___
#define ___Ashes_ColorSpace_HPP___
#pragma once

namespace ashes
{
	/**
	*\french
	*\brief
	*	Enumération des espaces de couleurs.
	*\english
	*\brief
	*	colour spaces enumeration.
	*/
	enum class ColorSpace
		: uint32_t
	{
		eSRGBNonLinear = 0,
		Ashes_EnumBounds( eSRGBNonLinear )
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
	inline std::string getName( ColorSpace value )
	{
		switch ( value )
		{
		case ColorSpace::eSRGBNonLinear:
			return "srgb_nonlinear";

		default:
			assert( false && "Unsupported ColorSpace." );
			throw std::runtime_error{ "Unsupported ColorSpace" };
		}

		return 0;
	}
}

#endif
