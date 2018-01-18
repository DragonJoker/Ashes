/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_TextureType_HPP___
#define ___Renderer_TextureType_HPP___
#pragma once

#include <Utils/UtilsPrerequisites.hpp>

namespace renderer
{
	/**
	*\brief
	*	Enumération des types de texture.
	*/
	enum class TextureType
		: uint32_t
	{
		e1D = 0,
		e2D = 1,
		e3D = 2,
		eCube = 3,
		e1DArray = 4,
		e2DArray = 5,
		eCubeArray = 6,
		Utils_EnumBounds( e1D )
	};
}

#endif
