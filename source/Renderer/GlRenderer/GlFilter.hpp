/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include <Renderer/Enum/Filter.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::Filter en VkFilter.
	*\param[in] filter
	*	Le renderer::Filter.
	*\return
	*	Le VkFilter.
	*/
	GLenum convert( renderer::Filter const & filter );
	/**
	*\brief
	*	Convertit un renderer::Filter en VkFilter, en tenant compte d'un mode de mipmapping.
	*\param[in] filter
	*	Le renderer::Filter.
	*\param[in] mode
	*	Le mode de mipmapping.
	*\return
	*	Le VkFilter.
	*/
	GLenum convert( renderer::Filter const & filter, renderer::MipmapMode mode );
}
