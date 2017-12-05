/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <VkLib/Viewport.hpp>
#include <Renderer/Viewport.hpp>

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
	vk::Viewport convert( renderer::Viewport const & viewport );
}
