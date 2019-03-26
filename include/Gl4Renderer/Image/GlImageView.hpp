/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_TextureView_HPP___
#define ___GlRenderer_TextureView_HPP___
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Image/ImageView.hpp>

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
	class ImageView
		: public ashes::ImageView
	{
	public:
		ImageView( Device const & device
			, Image const & image );
		ImageView( Device const & device
			, Image const & image
			, ashes::ImageViewCreateInfo const & createInfo );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~ImageView();
		/**
		*\return
		*	L'image OpenGL.
		*/
		GLuint getInternal()const noexcept;

	private:
		Device const & m_device;
		GlTextureViewType m_target;
		GLuint m_texture{ GL_INVALID_INDEX };
	};
}

#endif
