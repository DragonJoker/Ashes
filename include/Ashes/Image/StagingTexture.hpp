/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_StagingTexture_HPP___
#define ___Ashes_StagingTexture_HPP___
#pragma once

#include "Ashes/Buffer/Buffer.hpp"
#include "Ashes/Buffer/VertexBuffer.hpp"
#include "Ashes/Buffer/UniformBuffer.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	Class grouping the functions to transfer texture data from/to VRAM.
	*\~french
	*\brief
	*	Classe regroupant les fonctions nécessaires au transfert de données de texture depuis/vers la VRAM.
	*/
	class StagingTexture
	{
	public:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] extent
		*	The texture dimensions.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le LogicalDevice parent.
		*\param[in] extent
		*	Les dimensions de la texture.
		*/
		StagingTexture( Device const & device
			, Extent2D const & extent );
		virtual ~StagingTexture()
		{
		}
		/**
		*\name
		*	Upload.
		**/
		/**@{*/
		void uploadTextureData( CommandBuffer const & commandBuffer
			, ImageSubresourceLayers const & subresourceLayers
			, Format format
			, Offset3D const & offset
			, Extent2D const & extent
			, uint8_t const * const data
			, TextureView const & texture )const;
		inline void uploadTextureData( CommandBuffer const & commandBuffer
			, ImageSubresourceLayers const & subresourceLayers
			, Format format
			, Offset3D const & offset
			, Extent2D const & extent
			, ByteArray const & data
			, TextureView const & texture )const;
		void uploadTextureData( CommandBuffer const & commandBuffer
			, Format format
			, uint8_t const * const data
			, TextureView const & texture )const;
		inline void uploadTextureData( CommandBuffer const & commandBuffer
			, Format format
			, ByteArray const & data
			, TextureView const & texture )const;
		void copyTextureData( CommandBuffer const & commandBuffer
			, Format format
			, TextureView const & texture )const;
		void copyTextureData( CommandBuffer const & commandBuffer
			, ImageSubresourceLayers const & subresourceLayers
			, Format format
			, Offset3D const & offset
			, Extent2D const & extent
			, TextureView const & texture )const;
		/**@}*/
		/**
		*\name
		*	Download.
		**/
		/**@{*/
		void downloadTextureData( CommandBuffer const & commandBuffer
			, ImageSubresourceLayers const & subresourceLayers
			, Format format
			, Offset3D const & offset
			, Extent2D const & extent
			, uint8_t * data
			, TextureView const & texture )const;
		inline void downloadTextureData( CommandBuffer const & commandBuffer
			, ImageSubresourceLayers const & subresourceLayers
			, Format format
			, Offset3D const & offset
			, Extent2D const & extent
			, ByteArray & data
			, TextureView const & texture )const;
		void downloadTextureData( CommandBuffer const & commandBuffer
			, Format format
			, uint8_t * data
			, TextureView const & texture )const;
		inline void downloadTextureData( CommandBuffer const & commandBuffer
			, Format format
			, ByteArray & data
			, TextureView const & texture )const;
		/**@}*/
		/**
		*\~english
		*\brief
		*	Maps a range of the buffer's memory in RAM.
		*\param[in] size
		*	The range size.
		*\param[in] flags
		*	The mapping flags.
		*\return
		*	\p nullptr if mapping failed.
		*\~french
		*\brief
		*	Mappe la mémoire du tampon en RAM.
		*\param[in] size
		*	La taille en octets de la mémoire à mapper.
		*\param[in] flags
		*	Indicateurs de configuration du mapping.
		*\return
		*	\p nullptr si le mapping a échoué.
		*/
		virtual uint8_t * lock( Extent2D const & size
			, MemoryMapFlags flags )const = 0;
		/**
		*\~english
		*\brief
		*	Invalidates the buffer content.
		*\param[in] size
		*	The range size.
		*\~french
		*\brief
		*	Invalide le contenu du tampon.
		*\param[in] size
		*	La taille en octets de la mémoire mappée.
		*/
		virtual void invalidate( Extent2D const & size )const = 0;
		/**
		*\~english
		*\brief
		*	Updates the VRAM.
		*\param[in] size
		*	The range size.
		*\~french
		*\brief
		*	Met à jour la VRAM.
		*\param[in] size
		*	La taille en octets de la mémoire mappée.
		*/
		virtual void flush( Extent2D const & size )const = 0;
		/**
		*\~english
		*\brief
		*	Unmaps the buffer's memory from RAM.
		*\~french
		*\brief
		*	Unmappe la mémoire du tampon de la RAM.
		*/
		virtual void unlock()const = 0;

	private:
		/**
		*\name
		*	Upload.
		**/
		/**@{*/
		virtual void doCopyToStagingTexture( uint8_t const * const data
			, Format format
			, Extent2D const & extent )const = 0;
		virtual void doCopyStagingToDestination( CommandBuffer const & commandBuffer
			, ImageSubresourceLayers const & subresourceLayers
			, Format format
			, Offset3D const & offset
			, Extent2D const & extent
			, TextureView const & texture )const = 0;
		/**@{*/
		/**
		*\name
		*	Download.
		**/
		/**@{*/
		virtual void doCopyDestinationToStaging( CommandBuffer const & commandBuffer
			, ImageSubresourceLayers const & subresourceLayers
			, Format format
			, Offset3D const & offset
			, Extent2D const & extent
			, TextureView const & texture )const = 0;
		virtual void doCopyFromStagingTexture( uint8_t * data
			, Format format
			, Extent2D const & extent )const = 0;
		/**@}*/

	protected:
		Device const & m_device;
		Extent2D m_extent;
		AccessFlags m_currentAccessMask{ AccessFlag::eMemoryWrite };
	};
}

#include "Ashes/Image/StagingTexture.inl"

#endif
