/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DeviceCreateInfo_HPP___
#define ___Ashes_DeviceCreateInfo_HPP___
#pragma once

#include "Ashes/Miscellaneous/QueueCreateInfo.hpp"
#include "Ashes/Miscellaneous/PhysicalDeviceFeatures.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Holds the data used to create a logical device.
	*\~french
	*\brief
	*	Contient les données utilisées pour créer un périphérique logique.
	*/
	struct DeviceCreateInfo
	{
		DeviceCreateFlags flags{ DeviceCreateFlag::eNone };
		std::vector< DeviceQueueCreateInfo > queueCreateInfos;
		StringArray enabledLayerNames;
		StringArray enabledExtensionNames;
		PhysicalDeviceFeatures enabledFeatures;
	};
}

#endif
