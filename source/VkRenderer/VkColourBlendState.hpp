/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "VkColourBlendStateAttachment.hpp"

#include <VkLib/ColourBlendState.hpp>
#include <Renderer/ColourBlendState.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::ColourBlendState en vk::ColourBlendState.
	*\param[in] flags
	*	Le renderer::ColourBlendState.
	*\return
	*	Le vk::ColourBlendState.
	*/
	vk::ColourBlendState convert( renderer::ColourBlendState const & state );
}
