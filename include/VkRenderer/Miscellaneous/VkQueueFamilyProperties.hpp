/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/Miscellaneous/QueueFamilyProperties.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un VkQueueFamilyProperties en ashes::QueueFamilyProperties.
	*\param[in] value
	*	Le VkQueueFamilyProperties.
	*\return
	*	Le ashes::QueueFamilyProperties.
	*/
	ashes::QueueFamilyProperties convert( VkQueueFamilyProperties const & value );
}
