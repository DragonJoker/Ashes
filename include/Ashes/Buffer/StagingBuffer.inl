/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Command/CommandBuffer.hpp"
#include "Ashes/Command/CommandPool.hpp"
#include "Ashes/Command/Queue.hpp"
#include "Ashes/Sync/Fence.hpp"

namespace ashes
{
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
	template< typename T >
	inline void StagingBuffer::uploadBufferData( Queue const & queue
		, CommandPool const & commandPool
		, std::vector< T > const & data
		, Buffer< T > const & buffer )const
	{
		uploadBufferData( queue
			, commandPool
			, reinterpret_cast< uint8_t const * const >( data.data() )
			, uint32_t( data.size() * sizeof( T ) )
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::uploadBufferData( Queue const & queue
		, CommandPool const & commandPool
		, ByteArray const & data
		, Buffer< T > const & buffer )const
	{
		uploadBufferData( queue
			, commandPool
			, data.data()
			, uint32_t( data.size() )
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::uploadBufferData( Queue const & queue
		, CommandPool const & commandPool
		, uint8_t const * const data
		, uint32_t size
		, Buffer< T > const & buffer )const
	{
		uploadBufferData( queue
			, commandPool
			, data
			, size
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::uploadBufferData( Queue const & queue
		, CommandPool const & commandPool
		, std::vector< T > const & data
		, uint32_t offset
		, Buffer< T > const & buffer )const
	{
		uploadBufferData( queue
			, commandPool
			, reinterpret_cast< uint8_t const * const >( data.data() )
			, uint32_t( data.size() * sizeof( T ) )
			, uint32_t( offset * sizeof( T ) )
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::uploadBufferData( Queue const & queue
		, CommandPool const & commandPool
		, ByteArray const & data
		, uint32_t offset
		, Buffer< T > const & buffer )const
	{
		uploadBufferData( queue
			, commandPool
			, data.data()
			, uint32_t( data.size() )
			, offset
			, buffer );
	}

	template< typename T >
	void StagingBuffer::uploadBufferData( Queue const & queue
		, CommandPool const & commandPool
		, uint8_t const * const data
		, uint32_t size
		, uint32_t offset
		, Buffer< T > const & buffer )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( CommandBufferUsageFlag::eOneTimeSubmit );
		uploadBufferData( *commandBuffer
			, data
			, size
			, offset
			, buffer );
		commandBuffer->end();
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );
	}

	template< typename T >
	inline void StagingBuffer::uploadBufferData( CommandBuffer const & commandBuffer
		, std::vector< T > const & data
		, Buffer< T > const & buffer )const
	{
		uploadBufferData( commandBuffer
			, reinterpret_cast< uint8_t const * const >( data.data() )
			, uint32_t( data.size() * sizeof( T ) )
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::uploadBufferData( CommandBuffer const & commandBuffer
		, ByteArray const & data
		, Buffer< T > const & buffer )const
	{
		uploadBufferData( commandBuffer
			, data.data()
			, uint32_t( data.size() )
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::uploadBufferData( CommandBuffer const & commandBuffer
		, uint8_t const * const data
		, uint32_t size
		, Buffer< T > const & buffer )const
	{
		uploadBufferData( commandBuffer
			, data
			, size
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::uploadBufferData( CommandBuffer const & commandBuffer
		, std::vector< T > const & data
		, uint32_t offset
		, Buffer< T > const & buffer )const
	{
		uploadBufferData( commandBuffer
			, reinterpret_cast< uint8_t const * const >( data.data() )
			, uint32_t( data.size() * sizeof( T ) )
			, uint32_t( offset * sizeof( T ) )
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::uploadBufferData( CommandBuffer const & commandBuffer
		, ByteArray const & data
		, uint32_t offset
		, Buffer< T > const & buffer )const
	{
		uploadBufferData( commandBuffer
			, data.data()
			, uint32_t( data.size() )
			, offset
			, buffer );
	}

	template< typename T >
	void StagingBuffer::uploadBufferData( CommandBuffer const & commandBuffer
		, uint8_t const * const data
		, uint32_t size
		, uint32_t offset
		, Buffer< T > const & buffer )const
	{
		doCopyToStagingBuffer( data
			, size );
		doCopyFromStagingBuffer( commandBuffer
			, size
			, offset
			, buffer.getBuffer() );
	}
	/**@}*/
	/**
	*\name
	*	Vertex buffer.
	**/
	/**@{*/
	template< typename T >
	inline void StagingBuffer::uploadVertexData( Queue const & queue
		, CommandPool const & commandPool
		, std::vector< T > const & data
		, VertexBuffer< T > const & buffer )const
	{
		uploadVertexData( queue
			, commandPool
			, reinterpret_cast< uint8_t const * const >( data.data() )
			, uint32_t( data.size() * sizeof( T ) )
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::uploadVertexData( Queue const & queue
		, CommandPool const & commandPool
		, ByteArray const & data
		, VertexBuffer< T > const & buffer )const
	{
		uploadVertexData( queue
			, commandPool
			, data.data()
			, uint32_t( data.size() )
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::uploadVertexData( Queue const & queue
		, CommandPool const & commandPool
		, uint8_t const * const data
		, uint32_t size
		, VertexBuffer< T > const & buffer )const
	{
		uploadVertexData( queue
			, commandPool
			, data
			, size
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::uploadVertexData( Queue const & queue
		, CommandPool const & commandPool
		, std::vector< T > const & data
		, uint32_t offset
		, VertexBuffer< T > const & buffer )const
	{
		uploadVertexData( queue
			, commandPool
			, reinterpret_cast< uint8_t const * const >( data.data() )
			, uint32_t( data.size() * sizeof( T ) )
			, uint32_t( offset * sizeof( T ) )
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::uploadVertexData( Queue const & queue
		, CommandPool const & commandPool
		, ByteArray const & data
		, uint32_t offset
		, VertexBuffer< T > const & buffer )const
	{
		uploadVertexData( queue
			, commandPool
			, data.data()
			, uint32_t( data.size() )
			, offset
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::uploadVertexData( Queue const & queue
		, CommandPool const & commandPool
		, uint8_t const * const data
		, uint32_t size
		, uint32_t offset
		, VertexBuffer< T > const & buffer )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( CommandBufferUsageFlag::eOneTimeSubmit );
		uploadVertexData( *commandBuffer
			, data
			, size
			, offset
			, buffer );
		commandBuffer->end();
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );
	}

	template< typename T >
	inline void StagingBuffer::uploadVertexData( CommandBuffer const & commandBuffer
		, std::vector< T > const & data
		, VertexBuffer< T > const & buffer )const
	{
		uploadVertexData( commandBuffer
			, reinterpret_cast< uint8_t const * const >( data.data() )
			, uint32_t( data.size() * sizeof( T ) )
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::uploadVertexData( CommandBuffer const & commandBuffer
		, ByteArray const & data
		, VertexBuffer< T > const & buffer )const
	{
		uploadVertexData( commandBuffer
			, data.data()
			, uint32_t( data.size() )
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::uploadVertexData( CommandBuffer const & commandBuffer
		, uint8_t const * const data
		, uint32_t size
		, VertexBuffer< T > const & buffer )const
	{
		uploadVertexData( commandBuffer
			, data
			, size
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::uploadVertexData( CommandBuffer const & commandBuffer
		, std::vector< T > const & data
		, uint32_t offset
		, VertexBuffer< T > const & buffer )const
	{
		uploadVertexData( commandBuffer
			, reinterpret_cast< uint8_t const * const >( data.data() )
			, uint32_t( data.size() * sizeof( T ) )
			, uint32_t( offset * sizeof( T ) )
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::uploadVertexData( CommandBuffer const & commandBuffer
		, ByteArray const & data
		, uint32_t offset
		, VertexBuffer< T > const & buffer )const
	{
		uploadVertexData( commandBuffer
			, data.data()
			, uint32_t( data.size() )
			, offset
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::uploadVertexData( CommandBuffer const & commandBuffer
		, uint8_t const * const data
		, uint32_t size
		, uint32_t offset
		, VertexBuffer< T > const & buffer )const
	{
		doCopyToStagingBuffer( data
			, size );
		doCopyFromStagingBuffer( commandBuffer
			, size
			, offset
			, buffer );
	}
	/**@}*/
	/**
	*\name
	*	Uniform buffer.
	**/
	/**@{*/
	template< typename T >
	inline void StagingBuffer::uploadUniformData( Queue const & queue
		, CommandPool const & commandPool
		, std::vector< T > const & data
		, UniformBuffer< T > const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
		uploadUniformData( queue
			, commandPool
			, data.data()
			, uint32_t( data.size() )
			, 0u
			, buffer
			, dstStageFlags );
	}

	template< typename T >
	inline void StagingBuffer::uploadUniformData( Queue const & queue
		, CommandPool const & commandPool
		, T const * const data
		, uint32_t count
		, UniformBuffer< T > const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
		uploadUniformData( queue
			, commandPool
			, data
			, count
			, 0u
			, buffer
			, dstStageFlags );
	}

	template< typename T >
	inline void StagingBuffer::uploadUniformData( Queue const & queue
		, CommandPool const & commandPool
		, std::vector< T > const & data
		, uint32_t offset
		, UniformBuffer< T > const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
		uploadUniformData( queue
			, commandPool
			, data.data()
			, uint32_t( data.size() )
			, 0u
			, buffer
			, dstStageFlags );
	}

	template< typename T >
	inline void StagingBuffer::uploadUniformData( Queue const & queue
		, CommandPool const & commandPool
		, T const * const data
		, uint32_t count
		, uint32_t offset
		, UniformBuffer< T > const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( CommandBufferUsageFlag::eOneTimeSubmit );
		uploadUniformData( *commandBuffer
			, data
			, count
			, offset
			, buffer
			, dstStageFlags );
		commandBuffer->end();
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );
	}

	template< typename T >
	inline void StagingBuffer::uploadUniformData( CommandBuffer const & commandBuffer
		, std::vector< T > const & data
		, UniformBuffer< T > const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
		uploadUniformData( commandBuffer
			, data.data()
			, uint32_t( data.size() )
			, 0u
			, buffer
			, dstStageFlags );
	}

	template< typename T >
	inline void StagingBuffer::uploadUniformData( CommandBuffer const & commandBuffer
		, T const * const data
		, uint32_t count
		, UniformBuffer< T > const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
		uploadUniformData( commandBuffer
			, data
			, count
			, 0u
			, buffer
			, dstStageFlags );
	}

	template< typename T >
	inline void StagingBuffer::uploadUniformData( CommandBuffer const & commandBuffer
		, std::vector< T > const & data
		, uint32_t offset
		, UniformBuffer< T > const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
		uploadUniformData( commandBuffer
			, data.data()
			, uint32_t( data.size() )
			, 0u
			, buffer
			, dstStageFlags );
	}

	template< typename T >
	inline void StagingBuffer::uploadUniformData( CommandBuffer const & commandBuffer
		, T const * const data
		, uint32_t count
		, uint32_t offset
		, UniformBuffer< T > const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
		auto elemAlignedSize = buffer.getAlignedSize();
		doCopyUniformDataToStagingBuffer( data
			, count
			, elemAlignedSize );
		doCopyFromStagingBuffer( commandBuffer
			, elemAlignedSize * count
			, elemAlignedSize * offset
			, buffer.getUbo()
			, dstStageFlags );
	}
	/**@}*/
	/**
	*\name
	*	Texture.
	**/
	/**@{*/
	inline void StagingBuffer::uploadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, ImageSubresourceLayers const & subresourceLayers
		, Format format
		, Offset3D const & offset
		, Extent2D const & extent
		, ByteArray const & data
		, ImageView const & texture
		, PipelineStageFlags dstStageFlags )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( CommandBufferUsageFlag::eOneTimeSubmit );
		uploadTextureData( *commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, data.data()
			, texture
			, dstStageFlags );
		commandBuffer->end();
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );
	}

	inline void StagingBuffer::uploadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, Format format
		, ByteArray const & data
		, ImageView const & texture
		, PipelineStageFlags dstStageFlags )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( CommandBufferUsageFlag::eOneTimeSubmit );
		uploadTextureData( *commandBuffer
			, format
			, data.data()
			, texture
			, dstStageFlags );
		commandBuffer->end();
		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );
	}

	inline void StagingBuffer::uploadTextureData( CommandBuffer const & commandBuffer
		, ImageSubresourceLayers const & subresourceLayers
		, Format format
		, Offset3D const & offset
		, Extent2D const & extent
		, ByteArray const & data
		, ImageView const & texture
		, PipelineStageFlags dstStageFlags )const
	{
		uploadTextureData( commandBuffer
			, subresourceLayers
			, format
			, offset
			, extent
			, data.data()
			, texture
			, dstStageFlags );
	}

	inline void StagingBuffer::uploadTextureData( CommandBuffer const & commandBuffer
		, Format format
		, ByteArray const & data
		, ImageView const & texture
		, PipelineStageFlags dstStageFlags )const
	{
		uploadTextureData( commandBuffer
			, format
			, data.data()
			, texture
			, dstStageFlags );
	}
	/**@}*/
	/**@}*/
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
	template< typename T >
	inline void StagingBuffer::downloadBufferData( Queue const & queue
		, CommandPool const & commandPool
		, std::vector< T > & data
		, Buffer< T > const & buffer )const
	{
		downloadBufferData( queue
			, commandPool
			, reinterpret_cast< uint8_t const * const >( data.data() )
			, uint32_t( data.size() * sizeof( T ) )
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::downloadBufferData( Queue const & queue
		, CommandPool const & commandPool
		, ByteArray & data
		, Buffer< T > const & buffer )const
	{
		downloadBufferData( queue
			, commandPool
			, data.data()
			, uint32_t( data.size() )
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::downloadBufferData( Queue const & queue
		, CommandPool const & commandPool
		, uint8_t * data
		, uint32_t size
		, Buffer< T > const & buffer )const
	{
		downloadBufferData( queue
			, commandPool
			, data
			, size
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::downloadBufferData( Queue const & queue
		, CommandPool const & commandPool
		, std::vector< T > & data
		, uint32_t offset
		, Buffer< T > const & buffer )const
	{
		downloadBufferData( queue
			, commandPool
			, reinterpret_cast< uint8_t const * const >( data.data() )
			, uint32_t( data.size() * sizeof( T ) )
			, uint32_t( offset * sizeof( T ) )
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::downloadBufferData( Queue const & queue
		, CommandPool const & commandPool
		, ByteArray & data
		, uint32_t offset
		, Buffer< T > const & buffer )const
	{
		downloadBufferData( queue
			, commandPool
			, data.data()
			, uint32_t( data.size() )
			, offset
			, buffer );
	}

	template< typename T >
	void StagingBuffer::downloadBufferData( Queue const & queue
		, CommandPool const & commandPool
		, uint8_t * data
		, uint32_t size
		, uint32_t offset
		, Buffer< T > const & buffer )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( CommandBufferUsageFlag::eOneTimeSubmit );
		doCopyToStagingBuffer( *commandBuffer
			, size
			, offset
			, buffer.getBuffer() );
		commandBuffer->end();

		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );

		doCopyFromStagingBuffer( data
			, size );
	}
	/**@}*/
	/**
	*\name
	*	Vertex buffer.
	**/
	/**@{*/
	template< typename T >
	inline void StagingBuffer::downloadVertexData( Queue const & queue
		, CommandPool const & commandPool
		, std::vector< T > & data
		, VertexBuffer< T > const & buffer )const
	{
		downloadVertexData( queue
			, commandPool
			, reinterpret_cast< uint8_t const * const >( data.data() )
			, uint32_t( data.size() * sizeof( T ) )
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::downloadVertexData( Queue const & queue
		, CommandPool const & commandPool
		, ByteArray & data
		, VertexBuffer< T > const & buffer )const
	{
		downloadVertexData( queue
			, commandPool
			, data.data()
			, uint32_t( data.size() )
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::downloadVertexData( Queue const & queue
		, CommandPool const & commandPool
		, uint8_t * data
		, uint32_t size
		, VertexBuffer< T > const & buffer )const
	{
		downloadVertexData( queue
			, commandPool
			, data
			, size
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::downloadVertexData( Queue const & queue
		, CommandPool const & commandPool
		, std::vector< T > & data
		, uint32_t offset
		, VertexBuffer< T > const & buffer )const
	{
		downloadVertexData( queue
			, commandPool
			, reinterpret_cast< uint8_t const * const >( data.data() )
			, uint32_t( data.size() * sizeof( T ) )
			, uint32_t( offset * sizeof( T ) )
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::downloadVertexData( Queue const & queue
		, CommandPool const & commandPool
		, ByteArray & data
		, uint32_t offset
		, VertexBuffer< T > const & buffer )const
	{
		downloadVertexData( queue
			, commandPool
			, data.data()
			, uint32_t( data.size() )
			, offset
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::downloadVertexData( Queue const & queue
		, CommandPool const & commandPool
		, uint8_t * data
		, uint32_t size
		, uint32_t offset
		, VertexBuffer< T > const & buffer )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( CommandBufferUsageFlag::eOneTimeSubmit );
		doCopyToStagingBuffer( *commandBuffer
			, size
			, offset
			, buffer );
		commandBuffer->end();

		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );

		doCopyFromStagingBuffer( data
			, size );
	}
	/**@}*/
	/**
	*\name
	*	Uniform buffer.
	**/
	/**@{*/
	template< typename T >
	inline void StagingBuffer::downloadUniformData( Queue const & queue
		, CommandPool const & commandPool
		, std::vector< T > & data
		, UniformBuffer< T > const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
		downloadUniformData( queue
			, commandPool
			, data.data()
			, uint32_t( data.size() )
			, 0u
			, buffer
			, dstStageFlags );
	}

	template< typename T >
	inline void StagingBuffer::downloadUniformData( Queue const & queue
		, CommandPool const & commandPool
		, T * data
		, uint32_t count
		, UniformBuffer< T > const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
		downloadUniformData( queue
			, commandPool
			, data
			, count
			, 0u
			, buffer
			, dstStageFlags );
	}

	template< typename T >
	inline void StagingBuffer::downloadUniformData( Queue const & queue
		, CommandPool const & commandPool
		, std::vector< T > & data
		, uint32_t offset
		, UniformBuffer< T > const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
		downloadUniformData( queue
			, commandPool
			, data.data()
			, uint32_t( data.size() )
			, offset
			, buffer
			, dstStageFlags );
	}

	template< typename T >
	inline void StagingBuffer::downloadUniformData( Queue const & queue
		, CommandPool const & commandPool
		, T * data
		, uint32_t count
		, uint32_t offset
		, UniformBuffer< T > const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
		auto commandBuffer = commandPool.createCommandBuffer( true );
		commandBuffer->begin( CommandBufferUsageFlag::eOneTimeSubmit );

		auto elemAlignedSize = buffer.getAlignedSize();
		doCopyToStagingBuffer( *commandBuffer
			, elemAlignedSize * count
			, elemAlignedSize * offset
			, buffer.getUbo()
			, dstStageFlags );
		commandBuffer->end();

		auto fence = m_device.createFence();
		queue.submit( *commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );

		doCopyUniformDataFromStagingBuffer( data
			, count
			, elemAlignedSize );
	}
	/**@}*/
	/**
	*\name
	*	Texture.
	**/
	/**@{*/
	inline void StagingBuffer::downloadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, ImageSubresourceLayers const & subresourceLayers
		, Format format
		, Offset3D const & offset
		, Extent2D const & extent
		, ByteArray & data
		, ImageView const & texture
		, PipelineStageFlags dstStageFlags )const
	{
		downloadTextureData( queue
			, commandPool
			, subresourceLayers
			, format
			, offset
			, extent
			, data.data()
			, texture
			, dstStageFlags );
	}

	inline void StagingBuffer::downloadTextureData( Queue const & queue
		, CommandPool const & commandPool
		, Format format
		, ByteArray & data
		, ImageView const & texture
		, PipelineStageFlags dstStageFlags )const
	{
		downloadTextureData( queue
			, commandPool
			, format
			, data.data()
			, texture
			, dstStageFlags );
	}
	/**@}*/
	/**@}*/
	/**
	*\name
	*	Upload.
	**/
	/**@{*/
	template< typename T >
	inline void StagingBuffer::doCopyUniformDataToStagingBuffer( T const * const datas
		, uint32_t count
		, uint32_t elemAlignedSize )const
	{
		auto size = count * elemAlignedSize;
		ByteArray data( size_t( size ), uint8_t{} );
		auto buffer = data.data();

		for ( uint32_t i = 0; i < count; ++i )
		{
			std::memcpy( buffer, &datas[i], sizeof( T ) );
			buffer += elemAlignedSize;
		}

		doCopyToStagingBuffer( data.data(), size );
	}
	/**@}*/
	/**
	*\name
	*	Download.
	**/
	/**@{*/
	template< typename T >
	inline void StagingBuffer::doCopyUniformDataFromStagingBuffer( T * datas
		, uint32_t count
		, uint32_t offset )const
	{
		auto size = count * offset;
		ByteArray data( size_t( size ), uint8_t{} );
		doCopyFromStagingBuffer( data.data(), size );
		auto buffer = data.data();

		for ( uint32_t i = 0; i < count; ++i )
		{
			std::memcpy( &datas[i], buffer, sizeof( T ) );
			buffer += offset;
		}
	}
	/**@}*/
}
