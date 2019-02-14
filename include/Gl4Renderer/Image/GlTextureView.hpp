/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_TextureView_HPP___
#define ___GlRenderer_TextureView_HPP___
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

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
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~TextureView();
		/**
		*\return
		*	L'image OpenGL.
		*/
		GLuint getImage()const noexcept;

	private:
		Device const & m_device;
		GlTextureViewType m_target;
		GLuint m_texture{ GL_INVALID_INDEX };
	};
}

#endif
