/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/Miscellaneous/PhysicalDeviceMemoryProperties.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un VkPhysicalDeviceMemoryProperties en ashes::PhysicalDeviceMemoryProperties.
	*\param[in] value
	*	Le VkPhysicalDeviceMemoryProperties.
	*\return
	*	Le ashes::PhysicalDeviceMemoryProperties.
	*/
	ashes::PhysicalDeviceMemoryProperties convert( VkPhysicalDeviceMemoryProperties const & value );
}
