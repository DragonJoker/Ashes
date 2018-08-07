/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <Miscellaneous/SurfaceFormat.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::SurfaceFormat en VkSurfaceFormatKHR.
	*\param[in] value
	*	Le renderer::SurfaceFormat.
	*\return
	*	Le VkSurfaceFormatKHR.
	*/
	VkSurfaceFormatKHR convert( renderer::SurfaceFormat const & value );
	/**
	*\brief
	*	Convertit un VkSurfaceFormatKHR en renderer::SurfaceFormat.
	*\param[in] value
	*	Le VkSurfaceFormatKHR.
	*\return
	*	Le renderer::SurfaceFormat.
	*/
	renderer::SurfaceFormat convert( VkSurfaceFormatKHR const & value );
}
