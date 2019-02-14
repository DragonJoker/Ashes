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
	*	Convertit un ashes::BlendOp en VkBlendOp.
	*\param[in] value
	*	Le ashes::BlendOp.
	*\return
	*	Le VkBlendOp.
	*/
	VkBlendOp convert( ashes::BlendOp const & value );
}
