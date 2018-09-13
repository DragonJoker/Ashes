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
	*	Convertit un ashes::FrontFace en VkFrontFace.
	*\param[in] value
	*	Le ashes::FrontFace.
	*\return
	*	Le VkFrontFace.
	*/
	VkFrontFace convert( ashes::FrontFace const & value );
}
