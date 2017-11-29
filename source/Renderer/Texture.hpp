/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___Renderer_Texture_HPP___
#define ___Renderer_Texture_HPP___
#pragma once

#include "RendererPrerequisites.hpp"

#include <Utils/Vec2.hpp>

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
		*	Contient les informations d'une image mappée en RAM.
		*/
		struct Mapped
		{
			uint8_t * data;
			uint64_t size;
			uint64_t rowPitch;
			uint64_t arrayPitch;
			uint64_t depthPitch;
		};

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
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		Texture( Device const & device
			, vk::Image const & image );
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
		void setImage( utils::PixelFormat format
			, IVec2 const & size
			, ImageUsageFlags usageFlags = ImageUsageFlag::eTransferDst | ImageUsageFlag::eSampled
			, ImageTiling tiling = ImageTiling::eOptimal );
		/**
		*\brief
		*	Génère les mipmaps de la texture.
		*/
		void generateMipmaps()const noexcept;
		/**
		*\brief
		*	Mappe la mémoire du tampon en RAM.
		*\param[in] offset
		*	L'offset à partir duquel la mémoire du tampon est mappée.
		*\param[in] size
		*	La taille en octets de la mémoire à mapper.
		*\param[in] flags
		*	Indicateurs de configuration du mapping.
		*\return
		*	\p nullptr si le mapping a échoué.
		*/
		Mapped lock( uint32_t offset
			, uint32_t size
			, VkMemoryMapFlags flags )const;
		/**
		*\brief
		*	Unmappe la mémoire du tampon de la RAM.
		*\param[in] size
		*	La taille en octets de la mémoire mappée.
		*\param[in] modified
		*	Dit si le tampon a été modifié, et donc si la VRAM doit être mise à jour.
		*/
		void unlock( uint32_t size
			, bool modified )const;
		/**
		*\brief
		*	Active la texture.
		*\param[in] unit
		*	L'indice de l'unité sur laquelle la texture doit être activée.
		*/
		void bindAsShaderInput( CommandBuffer const & commandBuffer
			, uint32_t unit )const;
		/**
		*\brief
		*	Active la texture.
		*\param[in] unit
		*	L'indice de l'unité sur laquelle la texture doit être activée.
		*/
		void bindAsShaderOutput( CommandBuffer const & commandBuffer
			, uint32_t unit )const;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout général.
		*\param[in] accessFlags
		*	Les accès voulus, une fois que la transition est effectuée.
		*\return
		*	La barrière mémoire.
		*/
		ImageMemoryBarrier makeGeneralLayout( AccessFlags accessFlags )const;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de destination de transfert.
		*\return
		*	La barrière mémoire.
		*/
		ImageMemoryBarrier makeTransferDestination()const;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de source de transfert.
		*\return
		*	La barrière mémoire.
		*/
		ImageMemoryBarrier makeTransferSource()const;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de ressource d'entrée (lecture seule) d'un shader.
		*\return
		*	La barrière mémoire.
		*/
		ImageMemoryBarrier makeShaderInputResource()const;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout de ressource d'entrée (lecture seule) d'un shader.
		*\remarks
		*	Spécifique aux images prondeur et/ou stencil.
		*\return
		*	La barrière mémoire.
		*/
		ImageMemoryBarrier makeDepthStencilReadOnly()const;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout d'attache couleur.
		*\return
		*	La barrière mémoire.
		*/
		ImageMemoryBarrier makeColourAttachment()const;
		/**
		*\brief
		*	Prépare une barrière mémoire de transition vers un layout d'attache profondeur/stencil.
		*\return
		*	La barrière mémoire.
		*/
		ImageMemoryBarrier makeDepthStencilAttachment()const;
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
		/**
		*\return
		*	L'image vulkan.
		*/
		inline vk::Image const & getImage()const noexcept
		{
			assert( m_nonOwnedTexture );
			return *m_nonOwnedTexture;
		}

	private:
		Device const & m_device;
		utils::IVec2 m_size;
		utils::PixelFormat m_format{ utils::PixelFormat::eR8G8B8 };
		vk::ImagePtr m_ownedTexture;
		vk::Image const * m_nonOwnedTexture{ nullptr };
	};
}

#endif
