/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Renderer/Enum/StencilOp.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::StencilOp en VkStencilOp.
	*\param[in] value
	*	Le renderer::StencilOp.
	*\return
	*	Le VkStencilOp.
	*/
	GLenum convert( renderer::StencilOp const & value );
}
