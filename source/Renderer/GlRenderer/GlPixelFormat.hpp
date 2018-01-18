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
	*	Récupère le format interne OpenGL pour le renderer::PixelFormat donné.
	*\param[in] format
	*	Le renderer::PixelFormat.
	*\return
	*	Le format interne OpenGL.
	*/
	GLenum getInternal( renderer::PixelFormat const & format )noexcept;
	/**
	*\brief
	*	Récupère le format OpenGL pour le renderer::PixelFormat donné.
	*\param[in] format
	*	Le renderer::PixelFormat.
	*\return
	*	Le format OpenGL.
	*/
	GLenum getFormat( renderer::PixelFormat format )noexcept;
	/**
	*\brief
	*	Récupère le type de données OpenGL pour le renderer::PixelFormat donné.
	*\param[in] format
	*	Le renderer::PixelFormat.
	*\return
	*	Le type de données OpenGL.
	*/
	GLenum getType( renderer::PixelFormat format )noexcept;
	/**
	*\brief
	*	Récupère le taille des données pour le renderer::PixelFormat donné.
	*\param[in] format
	*	Le renderer::PixelFormat.
	*\return
	*	La taille des données.
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
