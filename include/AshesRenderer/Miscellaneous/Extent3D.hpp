/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_Extent3D_HPP___
#define ___Ashes_Extent3D_HPP___
#pragma once

#include "AshesRenderer/Miscellaneous/Extent2D.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies a three dimensional extent.
	*\~french
	*\brief
	*	Définit une étendue en 3 dimensions.
	*/
	struct Extent3D
	{
		uint32_t width;
		uint32_t height;
		uint32_t depth;
	};

	inline bool operator==( Extent3D const & lhs, Extent3D const & rhs )
	{
		return lhs.width == rhs.width
			&& lhs.height == rhs.height
			&& lhs.depth == rhs.depth;
	}

	inline bool operator!=( Extent3D const & lhs, Extent3D const & rhs )
	{
		return !( lhs == rhs );
	}
	/**
	*\~english
	*\brief
	*	Retrieves the minimal Extent2D for given pixel format.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The Extent2D.
	*\~french
	*\brief
	*	Donne l'Extent2D minimales pour le format de pixels donné.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	L'Extent2D.
	*/
	inline Extent3D getMinimalExtent3D( Format format )noexcept
	{
		auto const minimal = getMinimalExtent2D( format );
		return Extent3D{ minimal.width, minimal.height, 1 };
	}
	/**
	*\~english
	*\brief
	*	Checks if the given extent fits the given format.
	*\param[in] extent
	*	The dimensions.
	*\param[in] format
	*	The pixel format.
	*\return
	*	\p true if the extent is compatible with the format.
	*\~french
	*\brief
	*	Vérifie que les dimensiosn données sont compatibles avec le format donné.
	*\param[in] extent
	*	Les dimensions.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	\p true si les dimensions sont compatibles avec le format.
	*/
	inline bool checkExtent( Format format, Extent3D const & extent )
	{
		return checkExtent( format, Extent2D{ extent.width, extent.height } );
	}
	/**
	*\~english
	*\brief
	*	Retrieves the byte size of given pixel format and dimensions.
	*\param[in] extent
	*	The dimensions.
	*\param[in] format
	*	The pixel format.
	*\return
	*	The byte size.
	*\~french
	*\brief
	*	Donne le nombre d'octets du format de pixels et les dimensions donnés.
	*\param[in] extent
	*	Les dimensions.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le nombre d'octets.
	*/
	inline uint32_t getSize( Extent3D const & extent
		, Format format )noexcept
	{
		return getSize( Extent2D{ extent.width, extent.height }, format )
			* std::max( 1u, extent.depth );
	}

}

#endif
