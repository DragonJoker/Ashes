/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "VkRenderer/VkRendererPrerequisites.hpp"

#include <Ashes/Miscellaneous/SurfaceFormat.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::SurfaceFormat en VkSurfaceFormatKHR.
	*\param[in] value
	*	Le ashes::SurfaceFormat.
	*\return
	*	Le VkSurfaceFormatKHR.
	*/
	VkSurfaceFormatKHR convert( ashes::SurfaceFormat const & value );
	/**
	*\brief
	*	Convertit un VkSurfaceFormatKHR en ashes::SurfaceFormat.
	*\param[in] value
	*	Le VkSurfaceFormatKHR.
	*\return
	*	Le ashes::SurfaceFormat.
	*/
	ashes::SurfaceFormat convert( VkSurfaceFormatKHR const & value );
}
