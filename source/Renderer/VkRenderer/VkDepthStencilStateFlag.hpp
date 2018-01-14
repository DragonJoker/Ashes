/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Renderer/DepthStencilStateFlag.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::DepthStencilStateFlags en VkPipelineDepthStencilStateCreateFlags.
	*\param[in] flags
	*	Le renderer::DepthStencilStateFlags.
	*\return
	*	Le VkPipelineDepthStencilStateCreateFlags.
	*/
	VkPipelineDepthStencilStateCreateFlags convert( renderer::DepthStencilStateFlags const & flags );
}
