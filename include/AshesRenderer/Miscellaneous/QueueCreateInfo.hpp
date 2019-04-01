/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_DeviceQueueCreateInfo_HPP___
#define ___Ashes_DeviceQueueCreateInfo_HPP___
#pragma once

#include "AshesRenderer/AshesRendererPrerequisites.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Holds the data used to create a device queue.
	*\~french
	*\brief
	*	Contient les données utilisées pour créer une file.
	*/
	struct DeviceQueueCreateInfo
	{
		DeviceQueueCreateFlags flags;
		uint32_t queueFamilyIndex;
		std::vector< float > queuePriorities;
	};
}

#endif
