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
	*	Convertit un ashes::StencilOp en VkStencilOp.
	*\param[in] value
	*	Le ashes::StencilOp.
	*\return
	*	Le VkStencilOp.
	*/
	VkStencilOp convert( ashes::StencilOp const & value );
}
