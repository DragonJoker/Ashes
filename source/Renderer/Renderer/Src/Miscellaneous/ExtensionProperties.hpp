/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_ExtensionProperties_HPP___
#define ___Renderer_ExtensionProperties_HPP___
#pragma once

#include "Extent3D.hpp"

namespace renderer
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
