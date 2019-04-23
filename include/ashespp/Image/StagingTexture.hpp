/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_StagingTexture_HPP___
#define ___AshesPP_StagingTexture_HPP___
#pragma once

#include "ashespp/Buffer/Buffer.hpp"
#include "ashespp/Buffer/VertexBuffer.hpp"
#include "ashespp/Buffer/UniformBuffer.hpp"

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
			, VkFormat format
			, VkExtent2D const & extent );
		~StagingTexture()
		{
		}
		/**
		*\name
		*	Upload.
		**/
		/**@{*/
		void uploadTextureData( Queue const & queue
			, CommandPool const & commandPool
			, VkImageSubresourceLayers const & subresourceLayers
			, VkFormat format
			, VkOffset3D const & offset
			, VkExtent2D const & extent
			, uint8_t const * const data
			, ImageView const & texture )const;
		inline void uploadTextureData( Queue const & queue
			, CommandPool const & commandPool
			, VkImageSubresourceLayers const & subresourceLayers
			, VkFormat format
			, VkOffset3D const & offset
			, VkExtent2D const & extent
			, ByteArray const & data
			, ImageView const & texture )const;
		void uploadTextureData( Queue const & queue
			, CommandPool const & commandPool
			, VkFormat format
			, uint8_t const * const data
			, ImageView const & texture )const;
		inline void uploadTextureData( Queue const & queue
			, CommandPool const & commandPool
			, VkFormat format
			, ByteArray const & data
			, ImageView const & texture )const;
		void copyTextureData( Queue const & queue
			, CommandPool const & commandPool
			, VkFormat format
			, ImageView const & texture )const;
		void copyTextureData( Queue const & queue
			, CommandPool const & commandPool
			, VkImageSubresourceLayers const & subresourceLayers
			, VkFormat format
			, VkOffset3D const & offset
			, VkExtent2D const & extent
			, ImageView const & texture )const;
		void uploadTextureData( CommandBuffer const & commandBuffer
			, VkImageSubresourceLayers const & subresourceLayers
			, VkFormat format
			, VkOffset3D const & offset
			, VkExtent2D const & extent
			, uint8_t const * const data
			, ImageView const & texture )const;
		inline void uploadTextureData( CommandBuffer const & commandBuffer
			, VkImageSubresourceLayers const & subresourceLayers
			, VkFormat format
			, VkOffset3D const & offset
			, VkExtent2D const & extent
			, ByteArray const & data
			, ImageView const & texture )const;
		void uploadTextureData( CommandBuffer const & commandBuffer
			, VkFormat format
			, uint8_t const * const data
			, ImageView const & texture )const;
		inline void uploadTextureData( CommandBuffer const & commandBuffer
			, VkFormat format
			, ByteArray const & data
			, ImageView const & texture )const;
		void copyTextureData( CommandBuffer const & commandBuffer
			, VkFormat format
			, ImageView const & texture )const;
		void copyTextureData( CommandBuffer const & commandBuffer
			, VkImageSubresourceLayers const & subresourceLayers
			, VkFormat format
			, VkOffset3D const & offset
			, VkExtent2D const & extent
			, ImageView const & texture )const;
		/**@}*/
		/**
		*\name
		*	Download.
		**/
		/**@{*/
		void downloadTextureData( Queue const & queue
			, CommandPool const & commandPool
			, VkImageSubresourceLayers const & subresourceLayers
			, VkFormat format
			, VkOffset3D const & offset
			, VkExtent2D const & extent
			, uint8_t * data
			, ImageView const & texture )const;
		inline void downloadTextureData( Queue const & queue
			, CommandPool const & commandPool
			, VkImageSubresourceLayers const & subresourceLayers
			, VkFormat format
			, VkOffset3D const & offset
			, VkExtent2D const & extent
			, ByteArray & data
			, ImageView const & texture )const;
		void downloadTextureData( Queue const & queue
			, CommandPool const & commandPool
			, VkFormat format
			, uint8_t * data
			, ImageView const & texture )const;
		inline void downloadTextureData( Queue const & queue
			, CommandPool const & commandPool
			, VkFormat format
			, ByteArray & data
			, ImageView const & texture )const;
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
		uint8_t * lock( VkExtent2D const & size
			, VkMemoryMapFlags flags )const;
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
		void invalidate( VkExtent2D const & size )const;
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
		void flush( VkExtent2D const & size )const;
		/**
		*\~english
		*\brief
		*	Unmaps the buffer's memory from RAM.
		*\~french
		*\brief
		*	Unmappe la mémoire du tampon de la RAM.
		*/
		void unlock()const;

	private:
		/**
		*\name
		*	Upload.
		**/
		/**@{*/
		void doCopyToStagingTexture( uint8_t const * const data
			, VkFormat format
			, VkExtent2D const & extent )const;
		void doCopyStagingToDestination( CommandBuffer const & commandBuffer
			, VkImageSubresourceLayers const & subresourceLayers
			, VkFormat format
			, VkOffset3D const & offset
			, VkExtent2D const & extent
			, ImageView const & texture )const;
		/**@{*/
		/**
		*\name
		*	Download.
		**/
		/**@{*/
		void doCopyDestinationToStaging( CommandBuffer const & commandBuffer
			, VkImageSubresourceLayers const & subresourceLayers
			, VkFormat format
			, VkOffset3D const & offset
			, VkExtent2D const & extent
			, ImageView const & texture )const;
		void doCopyFromStagingTexture( uint8_t * data
			, VkFormat format
			, VkExtent2D const & extent )const;
		/**@}*/

	private:
		Device const & m_device;
		VkExtent2D m_extent;
		VkAccessFlags m_currentAccessMask{ VkAccessFlagBits::VK_ACCESS_MEMORY_WRITE_BIT };
		BufferBase m_buffer;
	};
}

#include "ashespp/Image/StagingTexture.inl"

#endif
