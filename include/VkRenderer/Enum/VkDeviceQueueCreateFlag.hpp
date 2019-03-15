/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::DeviceQueueCreateFlags en VkDeviceQueueCreateFlags.
	*\param[in] flags
	*	Le ashes::DeviceQueueCreateFlags.
	*\return
	*	Le VkDeviceQueueCreateFlags.
	*/
	VkDeviceQueueCreateFlags convert( ashes::DeviceQueueCreateFlags const & flags );
}
