/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Enum/MemoryPropertyFlag.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::MemoryPropertyFlags en GLbitfield.
	*\param[in] format
	*	Le renderer::MemoryPropertyFlags.
	*\return
	*	Le GLbitfield.
	*/
	GLbitfield convert( renderer::MemoryPropertyFlags const & flags );
}
