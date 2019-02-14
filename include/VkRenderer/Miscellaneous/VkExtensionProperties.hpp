/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_ExtensionProperties_HPP___
#define ___VkRenderer_ExtensionProperties_HPP___
#pragma once

#include "VkRenderer/VkRendererPrerequisites.hpp"

#include <Ashes/Miscellaneous/ExtensionProperties.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un VkExtensionProperties en ashes::ExtensionProperties.
	*\param[in] mode
	*	Le VkExtensionProperties.
	*\return
	*	Le ashes::ExtensionProperties.
	*/
	inline ashes::ExtensionProperties convert( VkExtensionProperties const & props )
	{
		return ashes::ExtensionProperties
		{
			props.extensionName,
			props.specVersion
		};
	}
}

#endif
