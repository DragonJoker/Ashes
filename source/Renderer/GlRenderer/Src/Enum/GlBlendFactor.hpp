/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Enum/BlendFactor.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::BlendFactor en VkBlendFactor.
	*\param[in] value
	*	Le renderer::BlendFactor.
	*\return
	*	Le VkBlendFactor.
	*/
	GLenum convert( renderer::BlendFactor const & value );
}
