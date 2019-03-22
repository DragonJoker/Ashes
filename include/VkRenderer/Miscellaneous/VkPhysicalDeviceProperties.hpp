/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/Miscellaneous/PhysicalDeviceProperties.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un VkPhysicalDeviceProperties en ashes::PhysicalDeviceProperties.
	*\param[in] value
	*	Le VkPhysicalDeviceProperties.
	*\return
	*	Le ashes::PhysicalDeviceProperties.
	*/
	ashes::PhysicalDeviceProperties convert( VkPhysicalDeviceProperties const & value );
}
