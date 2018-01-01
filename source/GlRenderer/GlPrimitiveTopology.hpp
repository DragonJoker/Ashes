/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include <Renderer/PrimitiveTopology.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::PrimitiveTopology en VkPrimitiveTopology.
	*\param[in] format
	*	Le renderer::PrimitiveTopology.
	*\return
	*	Le VkPrimitiveTopology.
	*/
	GLenum convert( renderer::PrimitiveTopology const & topology );
}
