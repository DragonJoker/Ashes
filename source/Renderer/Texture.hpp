/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Renderer_Texture_HPP___
#define ___Renderer_Texture_HPP___
#pragma once

#include "Vec2.hpp"

#include <VkLib/Image.hpp>
#include <VkLib/Sampler.hpp>

namespace renderer
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
		*/
		void setImage( PixelFormat format
			, IVec2 const & size
			, ByteArray const & data );
		/**
		*\brief
		*	Charge l'image de la texture.
		*\param[in] format
		*	Le format de l'image.
		*\param[in] size
		*	Les dimensions de l'image.
		*/
		void setImage( PixelFormat format
			, IVec2 const & size );
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
		void bindAsShaderInput( uint32_t unit )const;
		/**
		*\brief
		*	Active la texture.
		*\param[in] unit
		*	L'indice de l'unité sur laquelle la texture doit être activée.
		*/
		void bindAsShaderOutput( uint32_t unit )const;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de destination de transfert.
		*\return
		*	La barrière mémoire.
		*/
		ImageMemoryBarrier makeTransferDestination()const;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de ressource d'entrée (lecture seule) d'un shader.
		*\return
		*	La barrière mémoire.
		*/
		ImageMemoryBarrier makeShaderInputResource()const;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout d'attache couleur.
		*\return
		*	La barrière mémoire.
		*/
		ImageMemoryBarrier makeColourAttachment()const;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de destination de dessin.
		*\return
		*	La barrière mémoire.
		*/
		ImageMemoryBarrier makeDrawDestination()const;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de source de presentation.
		*\return
		*	La barrière mémoire.
		*/
		ImageMemoryBarrier makePresentSource()const;
		/**
		*\return
		*	Le format des pixels de la texture.
		*/
		inline renderer::PixelFormat getFormat()const noexcept
		{
			return m_format;
		}
		/**
		*\return
		*	Les dimensions de la texture.
		*/
		inline renderer::IVec2 const & getDimensions()const noexcept
		{
			return m_size;
		}
		/**
		*\return
		*	L'image vulkan.
		*/
		inline vk::Image const & getImage()const noexcept
		{
			assert( m_texture );
			return *m_texture;
		}

	private:
		renderer::RenderingResources const & m_resources;
		renderer::IVec2 m_size;
		renderer::PixelFormat m_format{ renderer::PixelFormat::eR8G8B8 };
		vk::ImagePtr m_texture;
	};
}

#endif
