/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <Renderer/Enum/ImageAspectFlag.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::ImageAspectFlags en VkImageAspectFlags.
	*\param[in] flags
	*	Le renderer::ImageAspectFlags.
	*\return
	*	Le VkImageAspectFlags.
	*/
	GLenum convert( renderer::ImageAspectFlags const & flags );
	/**
	*\brief
	*	Convertit un VkImageAspectFlags en renderer::ImageAspectFlags.
	*\param[in] flags
	*	Le VkImageAspectFlags.
	*\return
	*	Le renderer::ImageAspectFlags.
	*/
	renderer::ImageAspectFlags convertAspectMask( GLenum flags );
}
