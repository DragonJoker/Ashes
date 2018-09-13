/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Pipeline/ColourBlendState.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::ColourBlendState en VkPipelineColorBlendStateCreateInfo.
	*\param[in] flags
	*	Le ashes::ColourBlendState.
	*\return
	*	Le VkPipelineColorBlendStateCreateInfo.
	*/
	VkPipelineColorBlendStateCreateInfo convert( ashes::ColourBlendState const & state
		, std::vector< VkPipelineColorBlendAttachmentState > const & attaches );
}
