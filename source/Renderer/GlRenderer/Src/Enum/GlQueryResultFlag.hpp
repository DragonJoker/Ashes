/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Enum/QueryResultFlag.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::QueryResultFlags en GLuint.
	*\param[in] flags
	*	Le renderer::QueryResultFlags.
	*\return
	*	Le GLuint.
	*/
	GLuint convert( renderer::QueryResultFlags const & flags );
}
