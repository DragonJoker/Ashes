/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Enum/MemoryMapFlag.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::MemoryMapFlags en VkMemoryMapFlags.
	*\param[in] format
	*	Le renderer::MemoryMapFlags.
	*\return
	*	Le VkMemoryMapFlags.
	*/
	GLuint convert( renderer::MemoryMapFlags const & flags );
}
