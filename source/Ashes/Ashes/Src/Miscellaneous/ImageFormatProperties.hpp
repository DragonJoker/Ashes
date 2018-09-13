/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ImageFormatProperties_HPP___
#define ___Ashes_ImageFormatProperties_HPP___
#pragma once

#include "Extent3D.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies a image format properties.
	*\~french
	*\brief
	*	Définit les propriétés d'un format d'image.
	*/
	struct ImageFormatProperties
	{
		Extent3D maxExtent;
		uint32_t maxMipLevels;
		uint32_t maxArrayLayers;
		SampleCountFlags sampleCounts;
		uint64_t maxResourceSize;
	};
}

#endif
