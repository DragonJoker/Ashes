/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_StagingBuffer_HPP___
#define ___AshesPP_StagingBuffer_HPP___
#pragma once

#include "ashespp/Buffer/Buffer.hpp"
#include "ashespp/Buffer/VertexBuffer.hpp"
#include "ashespp/Buffer/UniformBuffer.hpp"

namespace ashes
{
	/**
	*\brief
	*	Class grouping the functions to transfer data from/to VRAM.
	*/
	class StagingBuffer
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] usage
		*	The buffer targets.
		*\param[in] size
		*	The buffer size.
		*/
		StagingBuffer( Device const & device
			, VkBufferUsageFlags usage
			, VkDeviceSize size = 10000000u
			, QueueShare const & sharingMode = {} );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] usage
		*	The buffer targets.
		*\param[in] size
		*	The buffer size.
		*/
		StagingBuffer( Device const & device
			, std::string const & debugName
			, VkBufferUsageFlags usage
			, VkDeviceSize size = 10000000u
			, QueueShare const & sharingMode = {} );
		/**
		*\name
		*	Upload.
		**/
		/**@{*/
		/**
		*\name
		*	Buffer.
		**/
		/**@{*/
		template< typename T, size_t N >
		void uploadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, std::array< T, N > const & data
			, Buffer< T > const & buffer )const;
		template< typename T >
		void uploadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, std::vector< T > const & data
			, Buffer< T > const & buffer )const;
		template< typename T >
		void uploadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, ByteArray const & data
			, Buffer< T > const & buffer )const;
		template< typename T >
		void uploadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, uint8_t const * const data
			, VkDeviceSize size
			, Buffer< T > const & buffer )const;
		template< typename T, size_t N >
		void uploadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, std::array< T, N > const & data
			, VkDeviceSize offset
			, Buffer< T > const & buffer )const;
		template< typename T >
		void uploadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, std::vector< T > const & data
			, VkDeviceSize offset
			, Buffer< T > const & buffer )const;
		template< typename T >
		void uploadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, ByteArray const & data
			, VkDeviceSize offset
			, Buffer< T > const & buffer )const;
		template< typename T >
		void uploadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, uint8_t const * const data
			, VkDeviceSize size
			, VkDeviceSize offset
			, Buffer< T > const & buffer )const;
		template< typename T, size_t N >
		void uploadBufferData( CommandBuffer const & commandBuffer
			, std::array< T, N > const & data
			, Buffer< T > const & buffer )const;
		template< typename T >
		void uploadBufferData( CommandBuffer const & commandBuffer
			, std::vector< T > const & data
			, Buffer< T > const & buffer )const;
		template< typename T >
		void uploadBufferData( CommandBuffer const & commandBuffer
			, ByteArray const & data
			, Buffer< T > const & buffer )const;
		template< typename T >
		void uploadBufferData( CommandBuffer const & commandBuffer
			, uint8_t const * const data
			, VkDeviceSize size
			, Buffer< T > const & buffer )const;
		template< typename T, size_t N >
		void uploadBufferData( CommandBuffer const & commandBuffer
			, std::array< T, N > const & data
			, VkDeviceSize offset
			, Buffer< T > const & buffer )const;
		template< typename T >
		void uploadBufferData( CommandBuffer const & commandBuffer
			, std::vector< T > const & data
			, VkDeviceSize offset
			, Buffer< T > const & buffer )const;
		template< typename T >
		void uploadBufferData( CommandBuffer const & commandBuffer
			, ByteArray const & data
			, VkDeviceSize offset
			, Buffer< T > const & buffer )const;
		template< typename T >
		void uploadBufferData( CommandBuffer const & commandBuffer
			, uint8_t const * const data
			, VkDeviceSize size
			, VkDeviceSize offset
			, Buffer< T > const & buffer )const;
		template< typename T, size_t N >
		void uploadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, std::array< T, N > const & data
			, BufferBase const & buffer )const;
		template< typename T >
		void uploadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, std::vector< T > const & data
			, BufferBase const & buffer )const;
		void uploadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, ByteArray const & data
			, BufferBase const & buffer )const;
		void uploadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, uint8_t const * const data
			, VkDeviceSize size
			, BufferBase const & buffer )const;
		template< typename T, size_t N >
		void uploadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, std::array< T, N > const & data
			, VkDeviceSize offset
			, BufferBase const & buffer )const;
		template< typename T >
		void uploadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, std::vector< T > const & data
			, VkDeviceSize offset
			, BufferBase const & buffer )const;
		void uploadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, ByteArray const & data
			, VkDeviceSize offset
			, BufferBase const & buffer )const;
		void uploadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, uint8_t const * const data
			, VkDeviceSize size
			, VkDeviceSize offset
			, BufferBase const & buffer )const;
		template< typename T, size_t N >
		void uploadBufferData( CommandBuffer const & commandBuffer
			, std::array< T, N > const & data
			, BufferBase const & buffer )const;
		template< typename T >
		void uploadBufferData( CommandBuffer const & commandBuffer
			, std::vector< T > const & data
			, BufferBase const & buffer )const;
		void uploadBufferData( CommandBuffer const & commandBuffer
			, ByteArray const & data
			, BufferBase const & buffer )const;
		void uploadBufferData( CommandBuffer const & commandBuffer
			, uint8_t const * const data
			, VkDeviceSize size
			, BufferBase const & buffer )const;
		template< typename T, size_t N >
		void uploadBufferData( CommandBuffer const & commandBuffer
			, std::array< T, N > const & data
			, VkDeviceSize offset
			, BufferBase const & buffer )const;
		template< typename T >
		void uploadBufferData( CommandBuffer const & commandBuffer
			, std::vector< T > const & data
			, VkDeviceSize offset
			, BufferBase const & buffer )const;
		void uploadBufferData( CommandBuffer const & commandBuffer
			, ByteArray const & data
			, VkDeviceSize offset
			, BufferBase const & buffer )const;
		void uploadBufferData( CommandBuffer const & commandBuffer
			, uint8_t const * const data
			, VkDeviceSize size
			, VkDeviceSize offset
			, BufferBase const & buffer )const;
		/**@}*/
		/**
		*\name
		*	Vertex buffer.
		**/
		/**@{*/
		template< typename T, size_t N >
		void uploadVertexData( Queue const & queue
			, CommandPool const & commandPool
			, std::array< T, N > const & data
			, VertexBuffer< T > const & buffer )const;
		template< typename T >
		void uploadVertexData( Queue const & queue
			, CommandPool const & commandPool
			, std::vector< T > const & data
			, VertexBuffer< T > const & buffer )const;
		template< typename T >
		void uploadVertexData( Queue const & queue
			, CommandPool const & commandPool
			, ByteArray const & data
			, VertexBuffer< T > const & buffer )const;
		template< typename T >
		void uploadVertexData( Queue const & queue
			, CommandPool const & commandPool
			, uint8_t const * const data
			, VkDeviceSize size
			, VertexBuffer< T > const & buffer )const;
		template< typename T, size_t N >
		void uploadVertexData( Queue const & queue
			, CommandPool const & commandPool
			, std::array< T, N > const & data
			, VkDeviceSize offset
			, VertexBuffer< T > const & buffer )const;
		template< typename T >
		void uploadVertexData( Queue const & queue
			, CommandPool const & commandPool
			, std::vector< T > const & data
			, VkDeviceSize offset
			, VertexBuffer< T > const & buffer )const;
		template< typename T >
		void uploadVertexData( Queue const & queue
			, CommandPool const & commandPool
			, ByteArray const & data
			, VkDeviceSize offset
			, VertexBuffer< T > const & buffer )const;
		template< typename T >
		void uploadVertexData( Queue const & queue
			, CommandPool const & commandPool
			, uint8_t const * const data
			, VkDeviceSize size
			, VkDeviceSize offset
			, VertexBuffer< T > const & buffer )const;
		template< typename T, size_t N >
		void uploadVertexData( CommandBuffer const & commandBuffer
			, std::array< T, N > const & data
			, VertexBuffer< T > const & buffer )const;
		template< typename T >
		void uploadVertexData( CommandBuffer const & commandBuffer
			, std::vector< T > const & data
			, VertexBuffer< T > const & buffer )const;
		template< typename T >
		void uploadVertexData( CommandBuffer const & commandBuffer
			, ByteArray const & data
			, VertexBuffer< T > const & buffer )const;
		template< typename T >
		void uploadVertexData( CommandBuffer const & commandBuffer
			, uint8_t const * const data
			, VkDeviceSize size
			, VertexBuffer< T > const & buffer )const;
		template< typename T, size_t N >
		void uploadVertexData( CommandBuffer const & commandBuffer
			, std::array< T, N > const & data
			, VkDeviceSize offset
			, VertexBuffer< T > const & buffer )const;
		template< typename T >
		void uploadVertexData( CommandBuffer const & commandBuffer
			, std::vector< T > const & data
			, VkDeviceSize offset
			, VertexBuffer< T > const & buffer )const;
		template< typename T >
		void uploadVertexData( CommandBuffer const & commandBuffer
			, ByteArray const & data
			, VkDeviceSize offset
			, VertexBuffer< T > const & buffer )const;
		template< typename T >
		void uploadVertexData( CommandBuffer const & commandBuffer
			, uint8_t const * const data
			, VkDeviceSize size
			, VkDeviceSize offset
			, VertexBuffer< T > const & buffer )const;
		/**@}*/
		/**
		*\name
		*	Uniform buffer.
		**/
		/**@{*/
		template< typename T, size_t N >
		void uploadUniformData( Queue const & queue
			, CommandPool const & commandPool
			, std::array< T, N > const & data
			, UniformBuffer const & buffer
			, VkPipelineStageFlags dstStageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT )const;
		template< typename T >
		void uploadUniformData( Queue const & queue
			, CommandPool const & commandPool
			, std::vector< T > const & data
			, UniformBuffer const & buffer
			, VkPipelineStageFlags dstStageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT )const;
		template< typename T >
		void uploadUniformData( Queue const & queue
			, CommandPool const & commandPool
			, T const * const data
			, VkDeviceSize count
			, UniformBuffer const & buffer
			, VkPipelineStageFlags dstStageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT )const;
		template< typename T, size_t N >
		void uploadUniformData( Queue const & queue
			, CommandPool const & commandPool
			, std::array< T, N > const & data
			, VkDeviceSize offset
			, UniformBuffer const & buffer
			, VkPipelineStageFlags dstStageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT )const;
		template< typename T >
		void uploadUniformData( Queue const & queue
			, CommandPool const & commandPool
			, std::vector< T > const & data
			, VkDeviceSize offset
			, UniformBuffer const & buffer
			, VkPipelineStageFlags dstStageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT )const;
		template< typename T >
		void uploadUniformData( Queue const & queue
			, CommandPool const & commandPool
			, T const * const data
			, VkDeviceSize count
			, VkDeviceSize offset
			, UniformBuffer const & buffer
			, VkPipelineStageFlags dstStageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT )const;
		template< typename T, size_t N >
		void uploadUniformData( CommandBuffer const & commandBuffer
			, std::array< T, N > const & data
			, UniformBuffer const & buffer
			, VkPipelineStageFlags dstStageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT )const;
		template< typename T >
		void uploadUniformData( CommandBuffer const & commandBuffer
			, std::vector< T > const & data
			, UniformBuffer const & buffer
			, VkPipelineStageFlags dstStageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT )const;
		template< typename T >
		void uploadUniformData( CommandBuffer const & commandBuffer
			, T const * const data
			, VkDeviceSize count
			, UniformBuffer const & buffer
			, VkPipelineStageFlags dstStageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT )const;
		template< typename T, size_t N >
		void uploadUniformData( CommandBuffer const & commandBuffer
			, std::array< T, N > const & data
			, VkDeviceSize offset
			, UniformBuffer const & buffer
			, VkPipelineStageFlags dstStageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT )const;
		template< typename T >
		void uploadUniformData( CommandBuffer const & commandBuffer
			, std::vector< T > const & data
			, VkDeviceSize offset
			, UniformBuffer const & buffer
			, VkPipelineStageFlags dstStageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT )const;
		template< typename T >
		void uploadUniformData( CommandBuffer const & commandBuffer
			, T const * const data
			, VkDeviceSize count
			, VkDeviceSize offset
			, UniformBuffer const & buffer
			, VkPipelineStageFlags dstStageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT )const;
		/**@}*/
		/**
		*\name
		*	Texture.
		**/
		/**@{*/
		void uploadTextureData( Queue const & queue
			, CommandPool const & commandPool
			, VkImageSubresourceLayers const & subresourceLayers
			, VkFormat format
			, VkOffset3D const & offset
			, VkExtent2D const & extent
			, uint8_t const * const data
			, ImageView const & texture
			, VkImageLayout dstLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )const;
		void uploadTextureData( Queue const & queue
			, CommandPool const & commandPool
			, VkImageSubresourceLayers const & subresourceLayers
			, VkFormat format
			, VkOffset3D const & offset
			, VkExtent2D const & extent
			, ByteArray const & data
			, ImageView const & texture
			, VkImageLayout dstLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )const;
		void uploadTextureData( Queue const & queue
			, CommandPool const & commandPool
			, VkImageSubresourceLayers const & subresourceLayers
			, VkFormat format
			, VkOffset3D const & offset
			, VkExtent2D const & extent
			, ArrayView< uint8_t > data
			, ImageView const & texture
			, VkImageLayout dstLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )const;
		void uploadTextureData( Queue const & queue
			, CommandPool const & commandPool
			, VkFormat format
			, uint8_t const * const data
			, ImageView const & texture
			, VkImageLayout dstLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )const;
		void uploadTextureData( Queue const & queue
			, CommandPool const & commandPool
			, VkFormat format
			, ByteArray const & data
			, ImageView const & texture
			, VkImageLayout dstLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )const;
		void uploadTextureData( CommandBuffer const & commandBuffer
			, VkImageSubresourceLayers const & subresourceLayers
			, VkFormat format
			, VkOffset3D const & offset
			, VkExtent2D const & extent
			, uint8_t const * const data
			, ImageView const & texture
			, VkImageLayout dstLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )const;
		void uploadTextureData( CommandBuffer const & commandBuffer
			, VkImageSubresourceLayers const & subresourceLayers
			, VkFormat format
			, VkOffset3D const & offset
			, VkExtent2D const & extent
			, ByteArray const & data
			, ImageView const & texture
			, VkImageLayout dstLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )const;
		void uploadTextureData( CommandBuffer const & commandBuffer
			, VkFormat format
			, uint8_t const * const data
			, ImageView const & texture
			, VkImageLayout dstLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )const;
		void uploadTextureData( CommandBuffer const & commandBuffer
			, VkFormat format
			, ByteArray const & data
			, ImageView const & texture
			, VkImageLayout dstLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )const;
		/**@}*/
		/**@}*/
		/**
		*\name
		*	Download.
		**/
		/**@{*/
		/**
		*\name
		*	Buffer.
		**/
		/**@{*/
		template< typename T >
		void downloadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, std::vector< T > & data
			, Buffer< T > const & buffer )const;
		template< typename T >
		void downloadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, ByteArray & data
			, Buffer< T > const & buffer )const;
		template< typename T >
		void downloadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, uint8_t * data
			, VkDeviceSize size
			, Buffer< T > const & buffer )const;
		template< typename T >
		void downloadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, std::vector< T > & data
			, VkDeviceSize offset
			, Buffer< T > const & buffer )const;
		template< typename T >
		void downloadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, ByteArray & data
			, VkDeviceSize offset
			, Buffer< T > const & buffer )const;
		template< typename T >
		void downloadBufferData( Queue const & queue
			, CommandPool const & commandPool
			, uint8_t * data
			, VkDeviceSize size
			, VkDeviceSize offset
			, Buffer< T > const & buffer )const;
		/**@}*/
		/**
		*\name
		*	Vertex buffer.
		**/
		/**@{*/
		template< typename T >
		void downloadVertexData( Queue const & queue
			, CommandPool const & commandPool
			, std::vector< T > & data
			, VertexBuffer< T > const & buffer )const;
		template< typename T >
		void downloadVertexData( Queue const & queue
			, CommandPool const & commandPool
			, ByteArray & data
			, VertexBuffer< T > const & buffer )const;
		template< typename T >
		void downloadVertexData( Queue const & queue
			, CommandPool const & commandPool
			, uint8_t * data
			, VkDeviceSize size
			, VertexBuffer< T > const & buffer )const;
		template< typename T >
		void downloadVertexData( Queue const & queue
			, CommandPool const & commandPool
			, std::vector< T > & data
			, VkDeviceSize offset
			, VertexBuffer< T > const & buffer )const;
		template< typename T >
		void downloadVertexData( Queue const & queue
			, CommandPool const & commandPool
			, ByteArray & data
			, VkDeviceSize offset
			, VertexBuffer< T > const & buffer )const;
		template< typename T >
		void downloadVertexData( Queue const & queue
			, CommandPool const & commandPool
			, uint8_t * data
			, VkDeviceSize size
			, VkDeviceSize offset
			, VertexBuffer< T > const & buffer )const;
		/**@}*/
		/**
		*\name
		*	Uniform buffer.
		**/
		/**@{*/
		template< typename T >
		void downloadUniformData( Queue const & queue
			, CommandPool const & commandPool
			, std::vector< T > & data
			, UniformBuffer const & buffer
			, VkPipelineStageFlags dstStageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT )const;
		template< typename T >
		void downloadUniformData( Queue const & queue
			, CommandPool const & commandPool
			, T * data
			, VkDeviceSize count
			, UniformBuffer const & buffer
			, VkPipelineStageFlags dstStageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT )const;
		template< typename T >
		void downloadUniformData( Queue const & queue
			, CommandPool const & commandPool
			, std::vector< T > & data
			, VkDeviceSize offset
			, UniformBuffer const & buffer
			, VkPipelineStageFlags dstStageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT )const;
		template< typename T >
		void downloadUniformData( Queue const & queue
			, CommandPool const & commandPool
			, T * data
			, VkDeviceSize count
			, VkDeviceSize offset
			, UniformBuffer const & buffer
			, VkPipelineStageFlags dstStageFlags = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT )const;
		/**@}*/
		/**
		*\name
		*	Texture.
		**/
		/**@{*/
		void downloadTextureData( Queue const & queue
			, CommandPool const & commandPool
			, VkImageSubresourceLayers const & subresourceLayers
			, VkFormat format
			, VkOffset3D const & offset
			, VkExtent2D const & extent
			, uint8_t * data
			, ImageView const & texture
			, VkImageLayout dstLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )const;
		void downloadTextureData( Queue const & queue
			, CommandPool const & commandPool
			, VkImageSubresourceLayers const & subresourceLayers
			, VkFormat format
			, VkOffset3D const & offset
			, VkExtent2D const & extent
			, ByteArray & data
			, ImageView const & texture
			, VkImageLayout dstLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )const;
		void downloadTextureData( Queue const & queue
			, CommandPool const & commandPool
			, VkFormat format
			, uint8_t * data
			, ImageView const & texture
			, VkImageLayout dstLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )const;
		void downloadTextureData( Queue const & queue
			, CommandPool const & commandPool
			, VkFormat format
			, ByteArray & data
			, ImageView const & texture
			, VkImageLayout dstLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )const;
		/**@}*/
		/**@}*/
		/**
		*\return
		*	Le tampon GPU.
		*/
		BufferBase const & getBuffer()const
		{
			return *m_buffer;
		}

	private:
		/**
		*\name
		*	Upload.
		**/
		/**@{*/
		template< typename T >
		void doCopyUniformDataToStagingBuffer( T const * const data
			, VkDeviceSize count
			, VkDeviceSize elemAlignedSize )const;
		void doCopyToStagingBuffer( uint8_t const * const data
			, VkDeviceSize size )const;
		void doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
			, VkDeviceSize size
			, VkDeviceSize offset
			, BufferBase const & buffer )const;
		void doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
			, VkDeviceSize size
			, VkDeviceSize offset
			, VertexBufferBase const & buffer )const;
		void doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
			, VkDeviceSize size
			, VkDeviceSize offset
			, UniformBuffer const & buffer
			, VkPipelineStageFlags dstStageFlags )const;
		void doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
			, VkExtent2D const & size
			, VkOffset3D const & offset
			, ImageView const & view
			, VkImageLayout dstLayout
			, VkImageSubresourceLayers const & subresourceLayers )const;
		/**@{*/
		/**
		*\name
		*	Download.
		**/
		/**@{*/
		template< typename T >
		void doCopyUniformDataFromStagingBuffer( T * data
			, VkDeviceSize count
			, VkDeviceSize elemAlignedSize )const;
		void doCopyFromStagingBuffer( uint8_t * data
			, VkDeviceSize size )const;
		void doCopyToStagingBuffer( CommandBuffer const & commandBuffer
			, VkDeviceSize size
			, VkDeviceSize offset
			, BufferBase const & buffer )const;
		void doCopyToStagingBuffer( CommandBuffer const & commandBuffer
			, VkDeviceSize size
			, VkDeviceSize offset
			, VertexBufferBase const & buffer )const;
		void doCopyToStagingBuffer( CommandBuffer const & commandBuffer
			, VkDeviceSize size
			, VkDeviceSize offset
			, UniformBuffer const & buffer
			, VkPipelineStageFlags dstStageFlags )const;
		void doCopyToStagingBuffer( CommandBuffer const & commandBuffer
			, VkExtent2D const & size
			, VkOffset3D const & offset
			, ImageView const & view
			, VkImageLayout dstLayout
			, VkImageSubresourceLayers const & subresourceLayers )const;
		/**@}*/

	private:
		Device const & m_device;
		BufferBasePtr m_buffer;
		DeviceMemoryPtr m_storage;
	};
}

#include "ashespp/Buffer/StagingBuffer.inl"

#endif
