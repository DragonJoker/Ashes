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

#include <VkLib/Image.hpp>
#include <VkLib/Sampler.hpp>

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
		*	Constructeur.
		*\param[in] resources
		*	Les ressources de rendu.
		*/
		Texture( RenderingResources const & resources );
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
		void image( renderer::PixelFormat format
			, renderer::IVec2 const & size
			, ByteArray const & data
			, vk::WrapMode wrapS = vk::WrapMode::eClampToEdge
			, vk::WrapMode wrapT = vk::WrapMode::eClampToEdge
			, vk::Filter minFilter = vk::Filter::eLinear
			, vk::Filter magFilter = vk::Filter::eLinear );
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
		inline renderer::PixelFormat format()const noexcept
		{
			return m_format;
		}
		/**
		*\return
		*	Les dimensions de la texture.
		*/
		inline renderer::IVec2 const & dimensions()const noexcept
		{
			return m_size;
		}

	private:
		//! Les ressources de rendu.
		RenderingResources const & m_resources;
		//! Les dimensions de l'image.
		renderer::IVec2 m_size;
		//! Le format des données de l'image.
		renderer::PixelFormat m_format{ renderer::PixelFormat::eR8G8B8 };
		//! La texture.
		vk::ImagePtr m_texture;
		//! L'échantillonneur.
		vk::SamplerPtr m_sampler;
	};
}

#endif
