/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Texture_HPP___
#define ___GlRenderer_Texture_HPP___
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <Image/Texture.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Une texture, avec son image et son échantillonneur.
	*/
	class Texture
		: public renderer::Texture
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		Texture( Device const & device
			, renderer::Format format
			, renderer::Extent2D const & dimensions );
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		Texture( Device const & device
			, renderer::ImageCreateInfo const & createInfo
			, renderer::MemoryPropertyFlags memoryFlags );
		/**
		*\brief
		*	Destructeur.
		*/
		~Texture();
		/**
		*\copydoc	renderer::Texture::createView
		*/
		renderer::TextureViewPtr createView( renderer::ImageViewCreateInfo const & createInfo )const override;
		/**
		*\copydoc	renderer::Texture::generateMipmaps
		*/
		void generateMipmaps()const override;

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
		inline renderer::SampleCountFlag getSamplesCount()const noexcept
		{
			return m_samples;
		}

	private:
		/**
		*\copydoc	renderer::Texture::doSetImage1D
		*/
		void doSetImage1D( uint32_t width );
		/**
		*\copydoc	renderer::Texture::doSetImage2D
		*/
		void doSetImage2D( uint32_t width
			, uint32_t height );
		/**
		*\copydoc	renderer::Texture::doSetImage3D
		*/
		void doSetImage3D( uint32_t width
			, uint32_t height
			, uint32_t depth );
		/**
		*\copydoc	renderer::Texture::doSetImage2D
		*/
		void doSetImage2DMS( uint32_t width
			, uint32_t height
			, renderer::SampleCountFlag samples );
		/**
		*\copydoc	renderer::Texture::doSetImage2D
		*/
		void doSetImage3DMS( uint32_t width
			, uint32_t height
			, uint32_t depth
			, renderer::SampleCountFlag samples );

	private:
		Device const & m_device;
		renderer::SampleCountFlag m_samples;
		GlTextureType m_target;
		GLuint m_texture{ GL_INVALID_INDEX };
	};
}

#endif
