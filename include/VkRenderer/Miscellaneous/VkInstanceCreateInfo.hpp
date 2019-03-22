/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/Miscellaneous/InstanceCreateInfo.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::InstanceCreateInfo en VkInstanceCreateInfo.
	*\param[in] mode
	*	Le ashes::InstanceCreateInfo.
	*\return
	*	Le VkInstanceCreateInfo.
	*/
	VkInstanceCreateInfo convert( ashes::InstanceCreateInfo const & value
		, VkApplicationInfo & applicationInfo
		, std::vector< char const * > & enabledLayerNames
		, std::vector< char const * > & enabledExtensionNames );
}

