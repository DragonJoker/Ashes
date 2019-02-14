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
	*	Convertit un ashes::LogicOp en VkLogicOp.
	*\param[in] value
	*	Le ashes::LogicOp.
	*\return
	*	Le VkLogicOp.
	*/
	VkLogicOp convert( ashes::LogicOp const & value );
}
