/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Enum/LogicOp.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::LogicOp en VkLogicOp.
	*\param[in] value
	*	Le renderer::LogicOp.
	*\return
	*	Le VkLogicOp.
	*/
	GLenum convert( renderer::LogicOp const & value );
}
