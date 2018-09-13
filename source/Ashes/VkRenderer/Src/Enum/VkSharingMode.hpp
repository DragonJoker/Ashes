/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::SharingMode en VkSharingMode.
	*\param[in] value
	*	Le ashes::SharingMode.
	*\return
	*	Le VkSharingMode.
	*/
	VkSharingMode convert( ashes::SharingMode const & value );
}
