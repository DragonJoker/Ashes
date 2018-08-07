/*
This file belongs to RendererLib.
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
	*	Convertit un renderer::SurfaceCapabilities en VkSurfaceCapabilitiesKHR.
	*\param[in] mode
	*	Le renderer::SurfaceCapabilities.
	*\return
	*	Le VkSurfaceCapabilitiesKHR.
	*/
	VkSurfaceCapabilitiesKHR convert( renderer::SurfaceCapabilities const & value );
	/**
	*\brief
	*	Convertit un VkSurfaceCapabilitiesKHR en renderer::SurfaceCapabilities.
	*\param[in] mode
	*	Le VkSurfaceCapabilitiesKHR.
	*\return
	*	Le renderer::SurfaceCapabilities.
	*/
	renderer::SurfaceCapabilities convert( VkSurfaceCapabilitiesKHR const & value );
}

#endif
