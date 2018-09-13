/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <AshesPrerequisites.hpp>

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
	VkPipelineStageFlags convert( ashes::PipelineStageFlags const & flags );
	/**
	*\brief
	*	Convertit un ashes::PipelineStageFlag en VkPipelineStageFlagBits.
	*\param[in] flags
	*	Le ashes::PipelineStageFlag.
	*\return
	*	Le VkPipelineStageFlagBits.
	*/
	VkPipelineStageFlagBits convert( ashes::PipelineStageFlag const & flags );
}
