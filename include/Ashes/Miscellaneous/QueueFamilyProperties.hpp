/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_QueueFamilyProperties_HPP___
#define ___Ashes_QueueFamilyProperties_HPP___
#pragma once

#include "Ashes/Miscellaneous/Extent3D.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Provides information about a queue family.
	*\~french
	*\brief
	*	Fournit des information à propos d'une famille de file.
	*/
	struct QueueFamilyProperties
	{
		QueueFlags queueFlags;
		uint32_t queueCount;
		uint32_t timestampValidBits;
		Extent3D minImageTransferGranularity;
	};
}

#endif
