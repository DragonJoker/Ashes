/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::ImageTiling en VkImageTiling.
	*\param[in] value
	*	Le ashes::ImageTiling.
	*\return
	*	Le VkImageTiling.
	*/
	VkImageTiling convert( ashes::ImageTiling const & value );
}
