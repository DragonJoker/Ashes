/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::TessellationStateFlags en VkPipelineTessellationStateCreateFlags.
	*\param[in] flags
	*	Le ashes::TessellationStateFlags.
	*\return
	*	Le VkPipelineTessellationStateCreateFlags.
	*/
	VkPipelineTessellationStateCreateFlags convert( ashes::TessellationStateFlags const & flags );
}
