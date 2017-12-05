/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <VkLib/MultisampleState.hpp>
#include <Renderer/MultisampleState.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::MultisampleState en vk::MultisampleState.
	*\param[in] state
	*	Le renderer::MultisampleState.
	*\return
	*	Le vk::MultisampleState.
	*/
	vk::MultisampleState convert( renderer::MultisampleState const & state );
}
