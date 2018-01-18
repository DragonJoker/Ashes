/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Renderer/Enum/ImageTiling.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::ImageTiling en VkImageTiling.
	*\param[in] value
	*	Le renderer::ImageTiling.
	*\return
	*	Le VkImageTiling.
	*/
	GLenum convert( renderer::ImageTiling const & value );
}
