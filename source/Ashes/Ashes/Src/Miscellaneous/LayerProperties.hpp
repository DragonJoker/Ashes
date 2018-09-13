/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_LayerProperties_HPP___
#define ___Ashes_LayerProperties_HPP___
#pragma once

#include "ExtensionProperties.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Specifies a layer properties.
	*\~french
	*\brief
	*	Définit les propriétés d'une couche.
	*/
	struct LayerProperties
	{
		std::string layerName;
		uint32_t specVersion{ 0u };
		uint32_t implementationVersion{ 0u };
		std::string description;
		std::vector< ExtensionProperties > extensions;
	};
}

#endif
