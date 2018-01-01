/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Renderer/AccessFlag.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::AccessFlags en VkAccessFlags.
	*\param[in] flags
	*	Le renderer::AccessFlags.
	*\return
	*	Le VkAccessFlags.
	*/
	GLenum convert( renderer::AccessFlags const & flags );
	/**
	*\brief
	*	Convertit un VkAccessFlags en renderer::AccessFlags.
	*\param[in] flags
	*	Le VkAccessFlags.
	*\return
	*	Le renderer::AccessFlags.
	*/
	renderer::AccessFlags convertAccessFlags( GLenum flags );
}
