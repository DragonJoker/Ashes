/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_SurfaceFormat_HPP___
#define ___Renderer_SurfaceFormat_HPP___
#pragma once

#include "RendererPrerequisites.hpp"

namespace renderer
{
	/**
	*\~french
	*\brief
	*	Format des pixels de la surface.
	*\~english
	*\brief
	*	Surface's pixel format.
	*/
	struct SurfaceFormat
	{
		Format format;
		ColorSpace colorSpace;
	};
}

#endif
