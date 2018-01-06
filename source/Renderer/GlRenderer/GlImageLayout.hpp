/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Renderer/ImageLayout.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::ImageLayout en VkImageLayout.
	*\param[in] layout
	*	Le renderer::ImageLayout.
	*\return
	*	Le VkImageLayout.
	*/
	GLenum convert( renderer::ImageLayout const & layout );
	/**
	*\brief
	*	Convertit un VkImageLayout en renderer::ImageLayout.
	*\param[in] layout
	*	Le VkImageLayout.
	*\return
	*	Le renderer::ImageLayout.
	*/
	renderer::ImageLayout convertImageLayout( GLenum layout );
}
