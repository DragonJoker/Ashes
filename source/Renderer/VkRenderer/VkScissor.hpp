/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <Renderer/Pipeline/Scissor.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::Viewport en vk::Viewport.
	*\param[in] flags
	*	Le renderer::Viewport.
	*\return
	*	Le vk::Viewport.
	*/
	VkRect2D convert( renderer::Scissor const & scissor );
}
