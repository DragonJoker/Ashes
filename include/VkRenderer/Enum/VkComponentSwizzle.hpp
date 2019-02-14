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
	*	Convertit un ashes::ComponentSwizzle en VkComponentSwizzle.
	*\param[in] value
	*	Le ashes::ComponentSwizzle.
	*\return
	*	Le VkComponentSwizzle.
	*/
	VkComponentSwizzle convert( ashes::ComponentSwizzle const & value );
}
