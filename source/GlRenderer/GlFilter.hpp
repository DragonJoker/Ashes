/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include <Renderer/Filter.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::Filter en VkFilter.
	*\param[in] filter
	*	Le renderer::Filter.
	*\return
	*	Le VkFilter.
	*/
	GLenum convert( renderer::Filter const & filter );
}
