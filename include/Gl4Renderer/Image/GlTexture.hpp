/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Texture_HPP___
#define ___GlRenderer_Texture_HPP___
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Image/Texture.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Une texture, avec son image et son échantillonneur.
	*/
	class Texture
		: public ashes::Texture
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		Texture( Device const & device
			, ashes::Format format
			, ashes::Extent2D const & dimensions );
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		Texture( Device const & device
			, ashes::ImageCreateInfo const & createInfo );
		/**
		*\brief
		*	Destructeur.
		*/
		~Texture();
		/**
		*\copydoc	ashes::Texture::getMemoryRequirements
		*/
		ashes::MemoryRequirements getMemoryRequirements()const override;
		/**
		*\copydoc	ashes::Texture::createView
		*/
		ashes::TextureViewPtr createView( ashes::ImageViewCreateInfo const & createInfo )const override;

		inline bool hasImage()const noexcept
		{
			return m_texture != GL_INVALID_INDEX;
		}
		/**
		*\return
		*	L'image OpenGL.
		*/
		inline GLuint getImage()const noexcept
		{
			assert( m_texture != GL_INVALID_INDEX );
			return m_texture;
		}
		/**
		*\return
		*	Le nombre d'échantillons.
		*/
		inline ashes::SampleCountFlag getSamplesCount()const noexcept
		{
			return m_createInfo.samples;
		}

	private:
		void doBindMemory()override;

	private:
		Device const & m_device;
		ashes::ImageCreateInfo m_createInfo;
		GlTextureType m_target;
		GLuint m_texture{ GL_INVALID_INDEX };
	};
}

#endif
