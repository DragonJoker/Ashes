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
	*\brief
	*	Class grouping the functions to transfer texture data from/to VRAM.
	*/
	class StagingTexture
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] extent
		*	The texture dimensions.
		*/
		StagingTexture( Device const & device
			, VkFormat format
			, VkExtent2D const & extent
			, uint32_t mipLevels = 1u );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] extent
		*	The texture dimensions.
		*/
		StagingTexture( Device const & device
			, std::string const & debugName
			, VkFormat format
			, VkExtent2D const & extent
			, uint32_t mipLevels = 1u );
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
			, uint8_t const * data
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
			, uint8_t const * data
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
			, uint8_t const * data
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
		*\brief
		*	Maps a range of the buffer's memory in RAM.
		*\param[in] size
		*	The range size.
		*\param[in] flags
		*	The mapping flags.
		*\return
		*	\p nullptr if mapping failed.
		*/
		uint8_t * lock( VkDeviceSize offset
			, VkDeviceSize size
			, VkMemoryMapFlags flags )const;
		/**
		*\brief
		*	Invalidates the buffer content.
		*\param[in] size
		*	The range size.
		*/
		void invalidate( VkDeviceSize offset
			, VkDeviceSize size )const;
		/**
		*\brief
		*	Updates the VRAM.
		*\param[in] size
		*	The range size.
		*/
		void flush( VkDeviceSize offset
			, VkDeviceSize size )const;
		/**
		*\brief
		*	Unmaps the buffer's memory from RAM.
		*/
		void unlock()const;

	private:
		/**
		*\name
		*	Upload.
		**/
		/**@{*/
		void doCopyToStagingTexture( uint8_t const * data
			, VkFormat format
			, VkExtent2D const & extent
			, uint32_t mipLevels )const;
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
		VkAccessFlags m_currentAccessMask{ VK_ACCESS_MEMORY_WRITE_BIT };
		BufferBase m_buffer;
	};
}

#include "ashespp/Image/StagingTexture.inl"

#endif
