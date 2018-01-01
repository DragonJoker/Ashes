/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Renderer/CullModeFlag.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::CullModeFlags en VkCullModeFlags.
	*\param[in] flags
	*	Le renderer::CullModeFlags.
	*\return
	*	Le VkCullModeFlags.
	*/
	GLenum convert( renderer::CullModeFlags const & flags );
}
