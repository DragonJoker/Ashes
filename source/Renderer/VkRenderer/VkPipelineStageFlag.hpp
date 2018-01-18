/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include <Renderer/Enum/PipelineStageFlag.hpp>

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
	VkPipelineStageFlags convert( renderer::PipelineStageFlags const & flags );
}
