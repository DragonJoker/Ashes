/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include <Enum/WrapMode.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::WrapMode en VkSamplerAddressMode.
	*\param[in] mode
	*	Le renderer::WrapMode.
	*\return
	*	Le VkSamplerAddressMode.
	*/
	GLenum convert( renderer::WrapMode const & mode );
}
