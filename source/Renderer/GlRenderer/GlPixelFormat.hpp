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
	*	Récupère le format interne OpenGL pour le utils::PixelFormat donné.
	*\param[in] format
	*	Le utils::PixelFormat.
	*\return
	*	Le format interne OpenGL.
	*/
	GLenum getInternal( utils::PixelFormat const & format )noexcept;
	/**
	*\brief
	*	Récupère le format OpenGL pour le utils::PixelFormat donné.
	*\param[in] format
	*	Le utils::PixelFormat.
	*\return
	*	Le format OpenGL.
	*/
	GLenum getFormat( utils::PixelFormat format )noexcept;
	/**
	*\brief
	*	Récupère le type de données OpenGL pour le utils::PixelFormat donné.
	*\param[in] format
	*	Le utils::PixelFormat.
	*\return
	*	Le type de données OpenGL.
	*/
	GLenum getType( utils::PixelFormat format )noexcept;
	/**
	*\brief
	*	Récupère le taille des données pour le utils::PixelFormat donné.
	*\param[in] format
	*	Le utils::PixelFormat.
	*\return
	*	La taille des données.
	*/
	uint32_t getSize( utils::PixelFormat format )noexcept;
	/**
	*\brief
	*	Convertit un VkFormat en utils::PixelFormat.
	*\param[in] format
	*	Le VkFormat.
	*\return
	*	Le utils::PixelFormat.
	*/
	utils::PixelFormat convert( GLenum format )noexcept;
}
