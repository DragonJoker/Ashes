#include "UniformBuffer.hpp"
#include "VertexBuffer.hpp"

namespace renderer
{
	template< typename T >
	void CommandBuffer::bindVertexBuffer( VertexBuffer< T > const & vertexBuffer
		, uint64_t offset )const
	{
		m_commandBuffer.bindVertexBuffer( vertexBuffer.getVbo()
			, offset );
	}

	template< typename T >
	void CommandBuffer::bindIndexBuffer( Buffer< T > const & indexBuffer
		, uint64_t offset
		, IndexType type )const
	{
		m_commandBuffer.bindIndexBuffer( indexBuffer.getBuffer()
			, offset
			, convert( type ) );
	}

	template< typename T >
	void CommandBuffer::copyBuffer( Buffer< T > const & src
		, Buffer< T > const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commandBuffer.copyBuffer( src.getBuffer()
			, dst.getBuffer()
			, size
			, offset );
	}

	template< typename T >
	void CommandBuffer::copyBuffer( Buffer< T > const & src
		, VertexBuffer< T > const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commandBuffer.copyBuffer( src.getBuffer()
			, dst.getVbo()
			, size
			, offset );
	}

	template< typename T >
	void CommandBuffer::copyBuffer( VertexBuffer< T > const & src
		, Buffer< T > const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commandBuffer.copyBuffer( src.getVbo()
			, dst.getBuffer()
			, size
			, offset );
	}

	template< typename T >
	void CommandBuffer::copyBuffer( VertexBuffer< T > const & src
		, VertexBuffer< T > const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commandBuffer.copyBuffer( src.getVbo()
			, dst.getVbo()
			, size
			, offset );
	}

	template< typename T >
	void CommandBuffer::copyBuffer( Buffer< T > const & src
		, UniformBuffer< T > const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commandBuffer.copyBuffer( src.getBuffer()
			, dst.getUbo()
			, size
			, offset );
	}

	template< typename T >
	void CommandBuffer::copyBuffer( UniformBuffer< T > const & src
		, Buffer< T > const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commandBuffer.copyBuffer( src.getUbo()
			, dst.getBuffer()
			, size
			, offset );
	}

	template< typename T >
	void CommandBuffer::copyBuffer( UniformBuffer< T > const & src
		, UniformBuffer< T > const & dst
		, uint32_t size
		, uint32_t offset )const
	{
		m_commandBuffer.copyBuffer( src.getUbo()
			, dst.getUbo()
			, size
			, offset );
	}

	template< typename T >
	void CommandBuffer::copyImage( Buffer< T > const & src
		, Texture const & dst )const
	{
		m_commandBuffer.copyImage( src.getBuffer()
			, dst.getImage() );
	}
}
