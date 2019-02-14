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
	*	Convertit un ashes::BlendFactor en VkBlendFactor.
	*\param[in] value
	*	Le ashes::BlendFactor.
	*\return
	*	Le VkBlendFactor.
	*/
	VkBlendFactor convert( ashes::BlendFactor const & value );
}
