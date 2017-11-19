/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_Texture_HPP___
#define ___RenderLib_Texture_HPP___
#pragma once

#include "RenderLibPrerequisites.h"

#include <GlLib/GlSampler.h>
#include <GlLib/GlTexture.h>

namespace render
{
	/**
	*\brief
	*	Une texture, avec son image et son échantillonneur.
	*/
	class Texture
	{
	public:
		/**
		*\brief
		*	Charge l'image de la texture.
		*\param[in] format
		*	Le format de l'image.
		*\param[in] size
		*	Les dimensions de l'image.
		*\param[in] data
		*	Les données de l'image.
		*\param[in] wrapS, wrapT
		*	Les modes de wrap de texture.
		*\param[in] minFilter, magFilter
		*	Les filtres de minification et magnification.
		*/
		void image( gl::PixelFormat format
			, gl::IVec2 const & size
			, ByteArray const & data
			, gl::WrapMode wrapS = gl::WrapMode::eClampToEdge
			, gl::WrapMode wrapT = gl::WrapMode::eClampToEdge
			, gl::MinFilter minFilter = gl::MinFilter::eLinear
			, gl::MagFilter magFilter = gl::MagFilter::eLinear );
		/**
		*\brief
		*	Génère les mipmaps de la texture.
		*/
		void generateMipmaps()const noexcept;
		/**
		*\brief
		*	Active la texture.
		*\param[in] unit
		*	L'indice de l'unité sur laquelle la texture doit être activée.
		*/
		void bind( uint32_t unit )const noexcept;
		/**
		*\brief
		*	Désactive la texture.
		*\param[in] unit
		*	L'indice de l'unité sur laquelle la texture a été activée.
		*/
		void unbind( uint32_t unit )const noexcept;
		/**
		*\return
		*	Le format des pixels de la texture.
		*/
		inline gl::PixelFormat format()const noexcept
		{
			return m_format;
		}
		/**
		*\return
		*	Les dimensions de la texture.
		*/
		inline gl::IVec2 const & dimensions()const noexcept
		{
			return m_size;
		}

	private:
		//! Les dimensions de l'image.
		gl::IVec2 m_size;
		//! Le format des données de l'image.
		gl::PixelFormat m_format{ gl::PixelFormat::eR8G8B8 };
		//! La texture OpenGL.
		gl::TexturePtr m_glTexture;
		//! L'échantillonneur OpenGL.
		gl::SamplerPtr m_glSampler;
	};
}

#endif
