/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_TextureView_HPP___
#define ___GlRenderer_TextureView_HPP___
#pragma once

#include "Gl3Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Image/TextureView.hpp>

namespace gl_renderer
{
	/**
	*\~french
	*\brief
	*	Description d'une vue sur une image Vulkan.
	*\~english
	*\brief
	*	Vulkan image view wrapper.
	*/
	class TextureView
		: public ashes::TextureView
	{
	public:
		TextureView( Device const & device
			, Texture const & image );
		TextureView( Device const & device
			, Texture const & image
			, ashes::ImageViewCreateInfo const & createInfo );
		~TextureView();

		inline GlTextureType getTarget()const noexcept
		{
			return m_target;
		}

	private:
		Device const & m_device;
		GlTextureType m_target;
	};
}

#endif
