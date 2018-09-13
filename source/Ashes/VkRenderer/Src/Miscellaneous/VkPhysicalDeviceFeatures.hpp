/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_PhysicalDeviceFeatures_HPP___
#define ___VkRenderer_PhysicalDeviceFeatures_HPP___
#pragma once

#include <Miscellaneous/PhysicalDeviceFeatures.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::PhysicalDeviceFeatures en VkPhysicalDeviceFeatures.
	*\param[in] mode
	*	Le ashes::PhysicalDeviceFeatures.
	*\return
	*	Le VkPhysicalDeviceFeatures.
	*/
	VkPhysicalDeviceFeatures convert( ashes::PhysicalDeviceFeatures const & value );
}

#endif
