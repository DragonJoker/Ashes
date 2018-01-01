/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#ifndef ___Renderer_Texture_HPP___
#define ___Renderer_Texture_HPP___
#pragma once

#include "RendererPrerequisites.hpp"

#include <Utils/PixelFormat.hpp>
#include <Utils/Vec2.hpp>

namespace renderer
{
	/**
	*\brief
	*	Une texture, avec son image et son échantillonneur.
	*/
	class Texture
	{
	protected:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		Texture( Device const & device );

	public:
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~Texture() = default;
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
		void setImage( utils::PixelFormat format
			, IVec2 const & size
			, ByteArray const & data
			, StagingBuffer const & stagingBuffer
			, CommandBuffer const & commandBuffer
			, ImageUsageFlags usageFlags = ImageUsageFlag::eTransferDst | ImageUsageFlag::eSampled
			, ImageTiling tiling = ImageTiling::eOptimal );
		/**
		*\brief
		*	Charge l'image de la texture.
		*\param[in] format
		*	Le format de l'image.
		*\param[in] size
		*	Les dimensions de l'image.
		*/
		virtual void setImage( utils::PixelFormat format
			, IVec2 const & size
			, ImageUsageFlags usageFlags = ImageUsageFlag::eTransferDst | ImageUsageFlag::eSampled
			, ImageTiling tiling = ImageTiling::eOptimal ) = 0;
		/**
		*\brief
		*	Génère les mipmaps de la texture.
		*/
		virtual void generateMipmaps()const = 0;
		/**
		*\brief
		*	Active la texture.
		*\param[in] unit
		*	L'indice de l'unité sur laquelle la texture doit être activée.
		*/
		virtual void bindAsShaderInput( CommandBuffer const & commandBuffer
			, uint32_t unit )const = 0;
		/**
		*\brief
		*	Active la texture.
		*\param[in] unit
		*	L'indice de l'unité sur laquelle la texture doit être activée.
		*/
		virtual void bindAsShaderOutput( CommandBuffer const & commandBuffer
			, uint32_t unit )const = 0;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout général.
		*\param[in] accessFlags
		*	Les accès voulus, une fois que la transition est effectuée.
		*\return
		*	La barrière mémoire.
		*/
		virtual ImageMemoryBarrier makeGeneralLayout( AccessFlags accessFlags )const = 0;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de destination de transfert.
		*\return
		*	La barrière mémoire.
		*/
		virtual ImageMemoryBarrier makeTransferDestination()const = 0;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de source de transfert.
		*\return
		*	La barrière mémoire.
		*/
		virtual ImageMemoryBarrier makeTransferSource()const = 0;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de ressource d'entrée (lecture seule) d'un shader.
		*\return
		*	La barrière mémoire.
		*/
		virtual ImageMemoryBarrier makeShaderInputResource()const = 0;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de ressource d'entrée (lecture seule) d'un shader.
		*\remarks
		*	Spécifique aux images prondeur et/ou stencil.
		*\return
		*	La barrière mémoire.
		*/
		virtual ImageMemoryBarrier makeDepthStencilReadOnly()const = 0;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout d'attache couleur.
		*\return
		*	La barrière mémoire.
		*/
		virtual ImageMemoryBarrier makeColourAttachment()const = 0;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout d'attache profondeur/stencil.
		*\return
		*	La barrière mémoire.
		*/
		virtual ImageMemoryBarrier makeDepthStencilAttachment()const = 0;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de source de presentation.
		*\return
		*	La barrière mémoire.
		*/
		virtual ImageMemoryBarrier makePresentSource()const = 0;
		/**
		*\return
		*	Le format des pixels de la texture.
		*/
		inline utils::PixelFormat getFormat()const noexcept
		{
			return m_format;
		}
		/**
		*\return
		*	Les dimensions de la texture.
		*/
		inline utils::IVec2 const & getDimensions()const noexcept
		{
			return m_size;
		}

	protected:
		Device const & m_device;
		utils::IVec2 m_size;
		utils::PixelFormat m_format{ utils::PixelFormat::eR8G8B8 };
	};
}

#endif
