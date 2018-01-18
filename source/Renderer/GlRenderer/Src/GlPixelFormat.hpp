/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include <Utils/PixelFormat.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	R�cup�re le format interne OpenGL pour le renderer::PixelFormat donn�.
	*\param[in] format
	*	Le renderer::PixelFormat.
	*\return
	*	Le format interne OpenGL.
	*/
	GLenum getInternal( renderer::PixelFormat const & format )noexcept;
	/**
	*\brief
	*	R�cup�re le format OpenGL pour le renderer::PixelFormat donn�.
	*\param[in] format
	*	Le renderer::PixelFormat.
	*\return
	*	Le format OpenGL.
	*/
	GLenum getFormat( renderer::PixelFormat format )noexcept;
	/**
	*\brief
	*	R�cup�re le type de donn�es OpenGL pour le renderer::PixelFormat donn�.
	*\param[in] format
	*	Le renderer::PixelFormat.
	*\return
	*	Le type de donn�es OpenGL.
	*/
	GLenum getType( renderer::PixelFormat format )noexcept;
	/**
	*\brief
	*	R�cup�re le taille des donn�es pour le renderer::PixelFormat donn�.
	*\param[in] format
	*	Le renderer::PixelFormat.
	*\return
	*	La taille des donn�es.
	*/
	uint32_t getSize( renderer::PixelFormat format )noexcept;
	/**
	*\brief
	*	Convertit un VkFormat en renderer::PixelFormat.
	*\param[in] format
	*	Le VkFormat.
	*\return
	*	Le renderer::PixelFormat.
	*/
	renderer::PixelFormat convert( GLenum format )noexcept;
}
