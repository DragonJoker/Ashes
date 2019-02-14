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
	*	Convertit un ashes::CompareOp en VkCompareOp.
	*\param[in] value
	*	Le ashes::CompareOp.
	*\return
	*	Le VkCompareOp.
	*/
	VkCompareOp convert( ashes::CompareOp const & value );
}
