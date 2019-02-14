/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::CommandBufferResetFlags en VkCommandBufferResetFlags.
	*\param[in] flags
	*	Le ashes::CommandBufferResetFlags.
	*\return
	*	Le VkCommandBufferResetFlags.
	*/
	VkCommandBufferResetFlags convert( ashes::CommandBufferResetFlags const & flags );
}
