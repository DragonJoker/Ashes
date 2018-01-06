/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <VkLib/ColourBlendStateAttachment.hpp>
#include <Renderer/ColourBlendStateAttachment.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::ColourBlendStateAttachment en vk::ColourBlendStateAttachment.
	*\param[in] flags
	*	Le renderer::ColourBlendState.
	*\return
	*	Le vk::ColourBlendStateAttachment.
	*/
	vk::ColourBlendStateAttachment convert( renderer::ColourBlendStateAttachment const & state );
}
