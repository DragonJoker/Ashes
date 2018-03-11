/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_Viewport_HPP___
#define ___Renderer_Viewport_HPP___
#pragma once

#include "Miscellaneous/Extent2D.hpp"
#include "Miscellaneous/Offset2D.hpp"

namespace renderer
{
	/**
	*\~french
	*\brief
	*	Description d'un viewport à utiliser lors de la création d'un pipeline ou à l'exécution.
	*\~english
	*\brief
	*	Speifies a viewport to use when creating a pipeline or executing a command buffer.
	*/
	struct Viewport
	{
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] width, height
		*	Les dimensions du viewport.
		*\param[in] x, y
		*	La position du viewport.
		*\param[in] minZ, maxZ
		*	Les bornes profondeur du viewport.
		*\~english
		*\brief
		*	Constructor.
		*\param[in] width, height
		*	The viewport dimensions.
		*\param[in] x, y
		*	The viewport position.
		*\param[in] minZ, maxZ
		*	The viewport depth bounds.
		*/
		Viewport( uint32_t width
			, uint32_t height
			, int32_t x
			, int32_t y
			, float minZ = 0.0f
			, float maxZ = 1.0f );

		Offset2D offset;
		Extent2D size;
		float minDepth;
		float maxDepth;
	};

	inline bool operator==( Viewport const & lhs, Viewport const & rhs )
	{
		return lhs.offset == rhs.offset
			&& lhs.size == rhs.size
			&& lhs.minDepth == rhs.minDepth
			&& lhs.maxDepth == rhs.maxDepth;
	}

	inline bool operator!=( Viewport const & lhs, Viewport const & rhs )
	{
		return !( lhs == rhs );
	}
}

#endif
