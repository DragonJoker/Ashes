/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_TextureType_HPP___
#define ___Ashes_TextureType_HPP___
#pragma once

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Texture types enumeration.
	*\~french
	*\brief
	*	Enumération des types de texture.
	*/
	enum class TextureType
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
	inline std::string getName( TextureType value )
	{
		switch ( value )
		{
		case TextureType::e1D:
			return "1d";

		case TextureType::e2D:
			return "2d";

		case TextureType::e3D:
			return "3d";

		default:
			assert( false && "Unsupported TextureType." );
			throw std::runtime_error{ "Unsupported TextureType" };
		}

		return 0;
	}
}

#endif
