/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_InstanceCreateInfo_HPP___
#define ___Ashes_InstanceCreateInfo_HPP___
#pragma once

#include "AshesRenderer/Miscellaneous/ApplicationInfo.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Holds the data used to create an instance.
	*\~french
	*\brief
	*	Contient les données utilisées pour créer une instance.
	*/
	struct InstanceCreateInfo
	{
		InstanceCreateFlags flags;
		ApplicationInfo applicationInfo;
		StringArray enabledLayerNames;
		StringArray enabledExtensionNames;
	};
}

#endif
