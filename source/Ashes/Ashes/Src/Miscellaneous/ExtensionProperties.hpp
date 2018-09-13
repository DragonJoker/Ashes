/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ExtensionProperties_HPP___
#define ___Ashes_ExtensionProperties_HPP___
#pragma once

#include "Extent3D.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies an extension properties.
	*\~french
	*\brief
	*	Définit les propriétés d'une extension.
	*/
	struct ExtensionProperties
	{
		std::string extensionName;
		uint32_t specVersion{ 0u };
	};
}

#endif
