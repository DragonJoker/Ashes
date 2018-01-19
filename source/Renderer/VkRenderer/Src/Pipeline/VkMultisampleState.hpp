/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <Pipeline/MultisampleState.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::MultisampleState en VkPipelineMultisampleStateCreateInfo.
	*\param[in] state
	*	Le renderer::MultisampleState.
	*\return
	*	Le VkPipelineMultisampleStateCreateInfo.
	*/
	VkPipelineMultisampleStateCreateInfo convert( renderer::MultisampleState const & state );
}
