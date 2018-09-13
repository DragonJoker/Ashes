/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_SurfaceCapabilities_HPP___
#define ___VkRenderer_SurfaceCapabilities_HPP___
#pragma once

#include <Miscellaneous/SurfaceCapabilities.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::SurfaceCapabilities en VkSurfaceCapabilitiesKHR.
	*\param[in] mode
	*	Le ashes::SurfaceCapabilities.
	*\return
	*	Le VkSurfaceCapabilitiesKHR.
	*/
	VkSurfaceCapabilitiesKHR convert( ashes::SurfaceCapabilities const & value );
	/**
	*\brief
	*	Convertit un VkSurfaceCapabilitiesKHR en ashes::SurfaceCapabilities.
	*\param[in] mode
	*	Le VkSurfaceCapabilitiesKHR.
	*\return
	*	Le ashes::SurfaceCapabilities.
	*/
	ashes::SurfaceCapabilities convert( VkSurfaceCapabilitiesKHR const & value );
}

#endif
