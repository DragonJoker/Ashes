/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_ExtensionProperties_HPP___
#define ___VkRenderer_ExtensionProperties_HPP___
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <Miscellaneous/ExtensionProperties.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un VkExtensionProperties en renderer::ExtensionProperties.
	*\param[in] mode
	*	Le VkExtensionProperties.
	*\return
	*	Le renderer::ExtensionProperties.
	*/
	inline renderer::ExtensionProperties convert( VkExtensionProperties const & props )
	{
		return renderer::ExtensionProperties
		{
			props.extensionName,
			props.specVersion
		};
	}
}

#endif
