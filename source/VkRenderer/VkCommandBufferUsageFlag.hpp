/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include <VkLib/VkLibPrerequisites.hpp>
#include <Renderer/CommandBufferUsageFlag.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::CommandBufferUsageFlag en VkCommandBufferUsageFlags.
	*\param[in] flags
	*	Le renderer::CommandBufferUsageFlag.
	*\return
	*	Le VkCommandBufferUsageFlags.
	*/
	VkCommandBufferUsageFlags convert( renderer::CommandBufferUsageFlags const & flags );
}
