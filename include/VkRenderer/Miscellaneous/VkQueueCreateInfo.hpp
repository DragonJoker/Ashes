/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_DeviceQueueCreateInfo_HPP___
#define ___VkRenderer_DeviceQueueCreateInfo_HPP___
#pragma once

#include <Ashes/Miscellaneous/QueueCreateInfo.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::DeviceQueueCreateInfo en VkDeviceQueueCreateInfo.
	*\param[in] mode
	*	Le ashes::DeviceQueueCreateInfo.
	*\return
	*	Le VkDeviceQueueCreateInfo.
	*/
	VkDeviceQueueCreateInfo convert( ashes::DeviceQueueCreateInfo const & value );
}

#endif
