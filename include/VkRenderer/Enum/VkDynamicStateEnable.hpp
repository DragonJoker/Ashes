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
	*	Convertit un ashes::DynamicStateEnable en VkDynamicStateEnable.
	*\param[in] value
	*	Le ashes::DynamicStateEnable.
	*\return
	*	Le VkDynamicStateEnable.
	*/
	VkDynamicStateEnable convert( ashes::DynamicStateEnable const & value );
}
