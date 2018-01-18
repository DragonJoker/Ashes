/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Renderer/Enum/PolygonMode.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::PolygonMode en VkPolygonMode.
	*\param[in] value
	*	Le renderer::PolygonMode.
	*\return
	*	Le VkPolygonMode.
	*/
	GLenum convert( renderer::PolygonMode const & value );
}
