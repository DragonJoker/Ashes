/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Pipeline/TessellationState.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::TessellationState en VkPipelineTessellationStateCreateInfo.
	*\param[in] state
	*	Le ashes::TessellationState.
	*\return
	*	Le VkPipelineTessellationStateCreateInfo.
	*/
	VkPipelineTessellationStateCreateInfo convert( ashes::TessellationState const & state );
}
