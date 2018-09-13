/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::PresentMode en VkPresentModeKHR.
	*\param[in] value
	*	Le ashes::PresentMode.
	*\return
	*	Le VkPresentModeKHR.
	*/
	VkPresentModeKHR convert( ashes::PresentMode const & value );
	/**
	*\brief
	*	Convertit un VkPresentModeKHR en ashes::PresentMode.
	*\param[in] value
	*	Le VkPresentModeKHR.
	*\return
	*	Le ashes::PresentMode.
	*/
	ashes::PresentMode convert( VkPresentModeKHR const & value );
}
