/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_LayerProperties_HPP___
#define ___Renderer_LayerProperties_HPP___
#pragma once

#include "ExtensionProperties.hpp"

namespace renderer
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
