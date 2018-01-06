/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Renderer/BlendOp.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::BlendOp en VkBlendOp.
	*\param[in] value
	*	Le renderer::BlendOp.
	*\return
	*	Le VkBlendOp.
	*/
	GLenum convert( renderer::BlendOp const & value );
}
