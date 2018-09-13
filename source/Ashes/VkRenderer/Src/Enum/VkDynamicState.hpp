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
	*	Convertit un ashes::DynamicState en VkDynamicState.
	*\param[in] value
	*	Le ashes::DynamicState.
	*\return
	*	Le VkDynamicState.
	*/
	VkDynamicState convert( ashes::DynamicState const & value );
}
