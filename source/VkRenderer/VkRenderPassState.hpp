/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <VkLib/VkLibPrerequisites.hpp>
#include <Renderer/RenderPassState.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::RenderPassState en vk::RenderPassState.
	*\param[in] state
	*	Le renderer::RenderPassState.
	*\return
	*	Le vk::RenderPassState.
	*/
	vk::RenderPassState convert( renderer::RenderPassState const & state );
}
