/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Pipeline/Scissor.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::Scissor en VkRect2D
	*\param[in] flags
	*	Le ashes::Scissor.
	*\return
	*	Le VkRect2D.
	*/
	VkRect2D convert( ashes::Scissor const & scissor );
}
