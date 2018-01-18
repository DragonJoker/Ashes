/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___GlRenderer_TextureType_HPP___
#define ___GlRenderer_TextureType_HPP___
#pragma once

#include <Renderer/RendererPrerequisites.hpp>

namespace renderer
{
	/**
	*\brief
	*	Convertit un renderer::WrapMode en VkSamplerAddressMode.
	*\param[in] mode
	*	Le renderer::WrapMode.
	*\return
	*	Le VkSamplerAddressMode.
	*/
	GLenum convert( renderer::TextureType const & mode );
}

#endif
