/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_SurfaceCapabilities_HPP___
#define ___Ashes_SurfaceCapabilities_HPP___
#pragma once

#include "Extent2D.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies the surface capabilities.
	*\~french
	*\brief
	*	Définit les capacités de la surface.
	*/
	struct SurfaceCapabilities
	{
		uint32_t minImageCount;
		uint32_t maxImageCount;
		Extent2D currentExtent;
		Extent2D minImageExtent;
		Extent2D maxImageExtent;
		uint32_t maxImageArrayLayers;
		SurfaceTransformFlags supportedTransforms;
		SurfaceTransformFlag currentTransform;
		CompositeAlphaFlags supportedCompositeAlpha;
		ImageUsageFlags supportedUsageFlags;
	};
}

#endif
