/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Renderer/Enum/BufferTarget.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::BufferTargets en VkBufferUsageFlags.
	*\param[in] targets
	*	Le renderer::BufferTargets.
	*\return
	*	Le VkBufferUsageFlags.
	*/
	GLenum convert( renderer::BufferTargets const & targets );
}
