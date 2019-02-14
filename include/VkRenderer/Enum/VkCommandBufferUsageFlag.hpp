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
	*	Convertit un ashes::CommandBufferUsageFlag en VkCommandBufferUsageFlags.
	*\param[in] flags
	*	Le ashes::CommandBufferUsageFlag.
	*\return
	*	Le VkCommandBufferUsageFlags.
	*/
	VkCommandBufferUsageFlags convert( ashes::CommandBufferUsageFlags const & flags );
}
