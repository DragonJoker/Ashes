/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_MipmapMode_HPP___
#define ___Renderer_MipmapMode_HPP___
#pragma once

#include "RendererPrerequisites.hpp"

namespace renderer
{
	/**
	*\brief
	*	Enumération des modes de mipmap.
	*/
	enum class MipmapMode
	{
		eNearest,
		eLinear,
		Utils_EnumBounds( eNearest )
	};
}

#endif
