/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ImageType_HPP___
#define ___Ashes_ImageType_HPP___
#pragma once

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Image types enumeration.
	*\~french
	*\brief
	*	Enumération des types de texture.
	*/
	enum class ImageType
		: uint32_t
	{
		e1D = 0,
		e2D = 1,
		e3D = 2,
		Ashes_EnumBounds( e1D )
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
	inline std::string getName( ImageType value )
	{
		switch ( value )
		{
		case ImageType::e1D:
			return "1d";

		case ImageType::e2D:
			return "2d";

		case ImageType::e3D:
			return "3d";

		default:
			assert( false && "Unsupported ImageType." );
			throw std::runtime_error{ "Unsupported ImageType" };
		}

		return 0;
	}
}

#endif
