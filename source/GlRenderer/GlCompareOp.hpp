/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Renderer/CompareOp.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::CompareOp en VkCompareOp.
	*\param[in] value
	*	Le renderer::CompareOp.
	*\return
	*	Le VkCompareOp.
	*/
	GLenum convert( renderer::CompareOp const & value );
}
