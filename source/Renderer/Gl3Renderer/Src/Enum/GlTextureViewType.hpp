/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___GlRenderer_TextureViewType_HPP___
#define ___GlRenderer_TextureViewType_HPP___
#pragma once

#include <RendererPrerequisites.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Convertit un renderer::WrapMode en GlTextureType.
	*\param[in] mode
	*	Le renderer::WrapMode.
	*\return
	*	Le GlTextureType.
	*/
	GlTextureType convert( renderer::TextureViewType const & mode );
}

#endif
