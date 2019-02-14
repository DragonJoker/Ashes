/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Pipeline/ViewportState.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::ViewportState en VkPipelineViewportStateCreateInfo.
	*\param[in] state
	*	Le ashes::ViewportState.
	*\return
	*	Le VkPipelineViewportStateCreateInfo.
	*/
	VkPipelineViewportStateCreateInfo convert( ashes::ViewportState const & state
		, std::vector< VkViewport > & viewports
		, std::vector< VkRect2D > & scissors );
}
