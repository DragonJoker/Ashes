/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <VkLib/TessellationState.hpp>
#include <Renderer/TessellationState.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::TessellationState en vk::TessellationState.
	*\param[in] state
	*	Le renderer::TessellationState.
	*\return
	*	Le vk::TessellationState.
	*/
	vk::TessellationState convert( renderer::TessellationState const & state );
}
