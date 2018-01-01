/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include <Renderer/MipmapMode.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::MipmapMode en VkSamplerMipmapMode.
	*\param[in] mode
	*	Le renderer::MipmapMode.
	*\return
	*	Le VkSamplerMipmapMode.
	*/
	GLenum convert( renderer::MipmapMode const & mode );
}
