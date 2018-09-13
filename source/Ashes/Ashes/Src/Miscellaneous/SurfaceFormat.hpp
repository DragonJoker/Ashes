/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_SurfaceFormat_HPP___
#define ___Ashes_SurfaceFormat_HPP___
#pragma once

#include "AshesPrerequisites.hpp"

namespace ashes
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
