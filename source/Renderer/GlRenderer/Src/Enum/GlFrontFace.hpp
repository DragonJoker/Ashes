/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Enum/FrontFace.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::FrontFace en VkFrontFace.
	*\param[in] value
	*	Le renderer::FrontFace.
	*\return
	*	Le VkFrontFace.
	*/
	GLenum convert( renderer::FrontFace const & value );
}
