/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Renderer/PolygonMode.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::PolygonMode en VkPolygonMode.
	*\param[in] value
	*	Le renderer::PolygonMode.
	*\return
	*	Le VkPolygonMode.
	*/
	VkPolygonMode convert( renderer::PolygonMode const & value );
}
