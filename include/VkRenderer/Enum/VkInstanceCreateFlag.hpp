/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::InstanceCreateFlags en VkInstanceCreateFlags.
	*\param[in] flags
	*	Le ashes::InstanceCreateFlags.
	*\return
	*	Le VkInstanceCreateFlags.
	*/
	VkInstanceCreateFlags convert( ashes::InstanceCreateFlags const & flags );
}
