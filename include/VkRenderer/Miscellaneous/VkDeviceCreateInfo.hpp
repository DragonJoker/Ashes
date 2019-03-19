/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/Miscellaneous/DeviceCreateInfo.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::DeviceCreateInfo en VkDeviceCreateInfo.
	*\param[in] mode
	*	Le ashes::DeviceCreateInfo.
	*\return
	*	Le VkDeviceCreateInfo.
	*/
	VkDeviceCreateInfo convert( ashes::DeviceCreateInfo const & value
		, std::vector< VkDeviceQueueCreateInfo > & queueCreateInfos
		, std::vector< char const * > & enabledLayerNames
		, std::vector< char const * > & enabledExtensionNames
		, VkPhysicalDeviceFeatures & enabledFeatures );
}

