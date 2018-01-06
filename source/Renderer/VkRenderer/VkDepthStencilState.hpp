/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "VkStencilOpState.hpp"

#include <VkLib/DepthStencilState.hpp>
#include <Renderer/DepthStencilState.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::DepthStencilState en vk::DepthStencilState.
	*\param[in] state
	*	Le renderer::DepthStencilState.
	*\return
	*	Le vk::DepthStencilState.
	*/
	vk::DepthStencilState convert( renderer::DepthStencilState const & state );
}
