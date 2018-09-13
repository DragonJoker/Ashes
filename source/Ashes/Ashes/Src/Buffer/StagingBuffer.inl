/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
namespace ashes
{
	/**
	*\name
	*	Upload.
	**/
	/**@{*/
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
	*	Upload.
	**/
	/**@{*/
	template< typename T >
	inline void StagingBuffer::downloadBufferData( CommandBuffer const & commandBuffer
		, std::vector< T > & data
		, Buffer< T > const & buffer )const
	{
		downloadBufferData( commandBuffer
			, reinterpret_cast< uint8_t const * const >( data.data() )
			, uint32_t( data.size() * sizeof( T ) )
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::downloadBufferData( CommandBuffer const & commandBuffer
		, ByteArray & data
		, Buffer< T > const & buffer )const
	{
		downloadBufferData( commandBuffer
			, data.data()
			, uint32_t( data.size() )
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::downloadBufferData( CommandBuffer const & commandBuffer
		, uint8_t * data
		, uint32_t size
		, Buffer< T > const & buffer )const
	{
		downloadBufferData( commandBuffer
			, data
			, size
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::downloadBufferData( CommandBuffer const & commandBuffer
		, std::vector< T > & data
		, uint32_t offset
		, Buffer< T > const & buffer )const
	{
		downloadBufferData( commandBuffer
			, reinterpret_cast< uint8_t const * const >( data.data() )
			, uint32_t( data.size() * sizeof( T ) )
			, uint32_t( offset * sizeof( T ) )
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::downloadBufferData( CommandBuffer const & commandBuffer
		, ByteArray & data
		, uint32_t offset
		, Buffer< T > const & buffer )const
	{
		downloadBufferData( commandBuffer
			, data.data()
			, uint32_t( data.size() )
			, offset
			, buffer );
	}

	template< typename T >
	void StagingBuffer::downloadBufferData( CommandBuffer const & commandBuffer
		, uint8_t * data
		, uint32_t size
		, uint32_t offset
		, Buffer< T > const & buffer )const
	{
		doCopyToStagingBuffer( commandBuffer
			, size
			, offset
			, buffer.getBuffer() );
		doCopyFromStagingBuffer( data
			, size );
	}

	template< typename T >
	inline void StagingBuffer::downloadVertexData( CommandBuffer const & commandBuffer
		, std::vector< T > & data
		, VertexBuffer< T > const & buffer )const
	{
		downloadVertexData( commandBuffer
			, reinterpret_cast< uint8_t const * const >( data.data() )
			, uint32_t( data.size() * sizeof( T ) )
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::downloadVertexData( CommandBuffer const & commandBuffer
		, ByteArray & data
		, VertexBuffer< T > const & buffer )const
	{
		downloadVertexData( commandBuffer
			, data.data()
			, uint32_t( data.size() )
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::downloadVertexData( CommandBuffer const & commandBuffer
		, uint8_t * data
		, uint32_t size
		, VertexBuffer< T > const & buffer )const
	{
		downloadVertexData( commandBuffer
			, data
			, size
			, 0u
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::downloadVertexData( CommandBuffer const & commandBuffer
		, std::vector< T > & data
		, uint32_t offset
		, VertexBuffer< T > const & buffer )const
	{
		downloadVertexData( commandBuffer
			, reinterpret_cast< uint8_t const * const >( data.data() )
			, uint32_t( data.size() * sizeof( T ) )
			, uint32_t( offset * sizeof( T ) )
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::downloadVertexData( CommandBuffer const & commandBuffer
		, ByteArray & data
		, uint32_t offset
		, VertexBuffer< T > const & buffer )const
	{
		downloadVertexData( commandBuffer
			, data.data()
			, uint32_t( data.size() )
			, offset
			, buffer );
	}

	template< typename T >
	inline void StagingBuffer::downloadVertexData( CommandBuffer const & commandBuffer
		, uint8_t * data
		, uint32_t size
		, uint32_t offset
		, VertexBuffer< T > const & buffer )const
	{
		doCopyToStagingBuffer( commandBuffer
			, size
			, offset
			, buffer );
		doCopyFromStagingBuffer( data
			, size );
	}

	template< typename T >
	inline void StagingBuffer::downloadUniformData( CommandBuffer const & commandBuffer
		, std::vector< T > & data
		, UniformBuffer< T > const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
		downloadUniformData( commandBuffer
			, data.data()
			, uint32_t( data.size() )
			, 0u
			, buffer
			, dstStageFlags );
	}

	template< typename T >
	inline void StagingBuffer::downloadUniformData( CommandBuffer const & commandBuffer
		, T * data
		, uint32_t count
		, UniformBuffer< T > const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
		downloadUniformData( commandBuffer
			, data
			, count
			, 0u
			, buffer
			, dstStageFlags );
	}

	template< typename T >
	inline void StagingBuffer::downloadUniformData( CommandBuffer const & commandBuffer
		, std::vector< T > & data
		, uint32_t offset
		, UniformBuffer< T > const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
		downloadUniformData( commandBuffer
			, data.data()
			, uint32_t( data.size() )
			, offset
			, buffer
			, dstStageFlags );
	}

	template< typename T >
	inline void StagingBuffer::downloadUniformData( CommandBuffer const & commandBuffer
		, T * data
		, uint32_t count
		, uint32_t offset
		, UniformBuffer< T > const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
		auto elemAlignedSize = buffer.getAlignedSize();
		doCopyToStagingBuffer( commandBuffer
			, elemAlignedSize * count
			, elemAlignedSize * offset
			, buffer.getUbo()
			, dstStageFlags );
		doCopyUniformDataFromStagingBuffer( data
			, count
			, elemAlignedSize );
	}
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
