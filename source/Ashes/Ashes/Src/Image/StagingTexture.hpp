/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_StagingTexture_HPP___
#define ___Ashes_StagingTexture_HPP___
#pragma once

#include "Buffer/Buffer.hpp"
#include "Buffer/VertexBuffer.hpp"
#include "Buffer/UniformBuffer.hpp"

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
			, Extent3D const & extent );
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
			, Extent3D const & extent
			, uint8_t const * const data
			, TextureView const & texture )const;
		inline void uploadTextureData( CommandBuffer const & commandBuffer
			, ImageSubresourceLayers const & subresourceLayers
			, Format format
			, Offset3D const & offset
			, Extent3D const & extent
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
			, Extent3D const & extent
			, uint8_t * data
			, TextureView const & texture )const;
		inline void downloadTextureData( CommandBuffer const & commandBuffer
			, ImageSubresourceLayers const & subresourceLayers
			, Format format
			, Offset3D const & offset
			, Extent3D const & extent
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

	private:
		/**
		*\name
		*	Upload.
		**/
		/**@{*/
		virtual void doCopyToStagingTexture( uint8_t const * const data
			, Format format
			, Extent3D const & extent )const = 0;
		virtual void doCopyStagingToDestination( CommandBuffer const & commandBuffer
			, ImageSubresourceLayers const & subresourceLayers
			, Format format
			, Offset3D const & offset
			, Extent3D const & extent
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
			, Extent3D const & extent
			, TextureView const & texture )const = 0;
		virtual void doCopyFromStagingTexture( uint8_t * data
			, Format format
			, Extent3D const & extent )const = 0;
		/**@}*/

	protected:
		Device const & m_device;
		Extent3D m_extent;
		AccessFlags m_currentAccessMask{ AccessFlag::eMemoryWrite };
	};
}

#include "StagingTexture.inl"

#endif
