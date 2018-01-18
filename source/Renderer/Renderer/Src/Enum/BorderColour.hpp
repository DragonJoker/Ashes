/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_BorderColour_HPP___
#define ___Renderer_BorderColour_HPP___
#pragma once

namespace renderer
{
	/**
	*\brief
	*	Enumération des types de texture.
	*/
	enum class BorderColour
		: uint32_t
	{
		eFloatTransparentBlack = 0,
		eIntTransparentBlack = 1,
		eFloatOpaqueBlack = 2,
		eIntOpaqueBlack = 3,
		eFloatOpaqueWhite = 4,
		eIntOpaqueWhite = 5,
		Utils_EnumBounds( eFloatTransparentBlack )
	};
}

#endif
