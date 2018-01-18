/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#pragma once

#include <Enum/AttributeFormat.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::AttributeFormat en VkFormat.
	*\param[in] flags
	*	Le renderer::AttributeFormat.
	*\return
	*	Le VkFormat.
	*/
	GLenum getType( renderer::AttributeFormat format );
	/**
	*\brief
	*	R�cup�re le nombre d'�l�ments du format donn�.
	*\param[in] flags
	*	Le renderer::AttributeFormat.
	*\return
	*	Le compte.
	*/
	uint32_t getCount( renderer::AttributeFormat format );
}
