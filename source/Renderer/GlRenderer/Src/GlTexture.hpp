/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___VkRenderer_Texture_HPP___
#define ___VkRenderer_Texture_HPP___
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <Image/Texture.hpp>

#include <Utils/Vec2.hpp>

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
		Texture( Device const & device );
		/**
		*\brief
		*	Destructeur.
		*/
		~Texture();
		/**
		*\brief
		*	Charge l'image de la texture.
		*\param[in] format
		*	Le format de l'image.
		*\param[in] size
		*	Les dimensions de l'image.
		*/
		void setImage( renderer::PixelFormat format
			, renderer::IVec2 const & size
			, renderer::ImageUsageFlags usageFlags = renderer::ImageUsageFlag::eTransferDst | renderer::ImageUsageFlag::eSampled
			, renderer::ImageTiling tiling = renderer::ImageTiling::eOptimal )override;
		/**
		*\brief
		*	Génère les mipmaps de la texture.
		*/
		void generateMipmaps()const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout général.
		*\param[in] accessFlags
		*	Les accès voulus, une fois que la transition est effectuée.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeGeneralLayout( renderer::AccessFlags accessFlags )const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de destination de transfert.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeTransferDestination()const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de source de transfert.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeTransferSource()const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de ressource d'entrée (lecture seule) d'un shader.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeShaderInputResource()const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de ressource d'entrée (lecture seule) d'un shader.
		*\remarks
		*	Spécifique aux images prondeur et/ou stencil.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeDepthStencilReadOnly()const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout d'attache couleur.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeColourAttachment()const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout d'attache profondeur/stencil.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makeDepthStencilAttachment()const override;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de source de presentation.
		*\return
		*	La barrière mémoire.
		*/
		renderer::ImageMemoryBarrier makePresentSource()const override;
		/**
		*\return
		*	L'image OpenGL.
		*/
		inline GLuint getImage()const noexcept
		{
			assert( m_texture != GL_INVALID_INDEX );
			return m_texture;
		}

	private:
		renderer::ImageMemoryBarrier doMakeLayoutTransition( renderer::ImageLayout layout
			, uint32_t queueFamily
			, renderer::AccessFlags dstAccessMask )const;

	private:
		Device const & m_device;
		GLuint m_texture{ GL_INVALID_INDEX };
	};
}

#endif
