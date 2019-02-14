/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Pipeline/ColourBlendStateAttachment.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::ColourBlendStateAttachment en VkPipelineColorBlendAttachmentState.
	*\param[in] flags
	*	Le ashes::ColourBlendState.
	*\return
	*	Le VkPipelineColorBlendAttachmentState.
	*/
	VkPipelineColorBlendAttachmentState convert( ashes::ColourBlendStateAttachment const & state );
}
