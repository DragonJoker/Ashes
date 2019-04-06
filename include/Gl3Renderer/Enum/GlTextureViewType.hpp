/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___GlRenderer_TextureViewType_HPP___
#define ___GlRenderer_TextureViewType_HPP___
#pragma once

#include <Ashes/AshesPrerequisites.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un ashes::WrapMode en GlTextureType.
	*\param[in] mode
	*	Le ashes::WrapMode.
	*\return
	*	Le GlTextureType.
	*/
	GlTextureType convert( VkImageViewType const & mode );
	GlTextureType convert( VkImageViewType const & mode
		, uint32_t layers );
}

#endif
