/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <VkLib/VkLibPrerequisites.hpp>
#include <Renderer/RenderSubpassState.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::RenderSubpassState en vk::RenderSubpassState.
	*\param[in] state
	*	Le renderer::RenderSubpassState.
	*\return
	*	Le vk::RenderSubpassState.
	*/
	vk::RenderSubpassState convert( renderer::RenderSubpassState const & state );
}
