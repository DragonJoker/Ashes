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
	*	Convertit un ashes::ColorSpace en VkColorSpaceKHR.
	*\param[in] value
	*	Le ashes::ColorSpace.
	*\return
	*	Le VkColorSpaceKHR.
	*/
	VkColorSpaceKHR convert( ashes::ColorSpace const & value );
	/**
	*\brief
	*	Convertit un VkColorSpaceKHR en ashes::ColorSpace.
	*\param[in] value
	*	Le VkColorSpaceKHR.
	*\return
	*	Le ashes::ColorSpace.
	*/
	ashes::ColorSpace convertColorSpace( VkColorSpaceKHR const & value );
}
