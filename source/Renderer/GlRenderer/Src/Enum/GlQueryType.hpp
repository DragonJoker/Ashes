/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Enum/QueryType.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::QueryType en GLuint.
	*\param[in] value
	*	Le renderer::QueryType.
	*\return
	*	Le GLuint.
	*/
	GLenum convert( renderer::QueryType const & value );
}
