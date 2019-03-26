/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ImageViewType_HPP___
#define ___Ashes_ImageViewType_HPP___
#pragma once

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Image view types enumeration.
	*\~french
	*\brief
	*	Enumération des types de vue sur une texture.
	*/
	enum class ImageViewType
		: uint32_t
	{
		e1D = 0,
		e2D = 1,
		e3D = 2,
		eCube = 3,
		e1DArray = 4,
		e2DArray = 5,
		eCubeArray = 6,
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
	inline std::string getName( ImageViewType value )
	{
		switch ( value )
		{
		case ImageViewType::e1D:
			return "1d";

		case ImageViewType::e2D:
			return "2d";

		case ImageViewType::e3D:
			return "3d";

		case ImageViewType::eCube:
			return "cube";

		case ImageViewType::e1DArray:
			return "1d_array";

		case ImageViewType::e2DArray:
			return "2d_array";

		case ImageViewType::eCubeArray:
			return "cube_array";

		default:
			assert( false && "Unsupported ImageViewType." );
			throw std::runtime_error{ "Unsupported ImageViewType" };
		}

		return 0;
	}
}

#endif
