/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_SwapChainCreateInfo_HPP___
#define ___Ashes_SwapChainCreateInfo_HPP___
#pragma once

#include "Ashes/AshesPrerequisites.hpp"
#include "Ashes/Miscellaneous/Extent2D.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Holds the data used to create a swapchain.
	*\~french
	*\brief
	*	Contient les données utilisées pour créer une swapchain.
	*/
	struct SwapChainCreateInfo
	{
		SwapChainCreateFlags flags;
		SurfaceCRef surface;
		uint32_t minImageCount;
		Format imageFormat;
		ColorSpace imageColorSpace;
		Extent2D imageExtent;
		uint32_t imageArrayLayers;
		ImageUsageFlags imageUsage;
		SharingMode imageSharingMode;
		std::vector< uint32_t > queueFamilyIndices;
		SurfaceTransformFlag preTransform;
		CompositeAlphaFlag compositeAlpha;
		PresentMode presentMode;
		bool clipped;
		std::optional< SwapChainCRef > oldSwapchain;
	};
}

#endif
