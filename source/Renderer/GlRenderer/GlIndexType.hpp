/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include <Renderer/Enum/IndexType.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::IndexType en VkIndexType.
	*\param[in] type
	*	Le renderer::IndexType.
	*\return
	*	Le VkIndexType.
	*/
	GLenum convert( renderer::IndexType const & type );
}
