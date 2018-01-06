/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include <VkLib/VkLibPrerequisites.hpp>
#include <Renderer/CommandBufferResetFlag.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::CommandBufferResetFlags en VkCommandBufferResetFlags.
	*\param[in] flags
	*	Le renderer::CommandBufferResetFlags.
	*\return
	*	Le VkCommandBufferResetFlags.
	*/
	VkCommandBufferResetFlags convert( renderer::CommandBufferResetFlags const & flags );
}
