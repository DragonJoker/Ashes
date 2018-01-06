/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "StagingBuffer.hpp"

#include "BufferMemoryBarrier.hpp"
#include "ImageMemoryBarrier.hpp"
#include "Texture.hpp"

namespace renderer
{
	StagingBuffer::StagingBuffer( Device const & device
		, BufferTargets target
		, uint32_t size )
		: m_device{ device }
		, m_buffer{ device.createBuffer( size
			, target | BufferTarget::eTransferSrc
			, MemoryPropertyFlag::eHostVisible ) }
	{
	}

	void StagingBuffer::doCopyToStagingBuffer( uint8_t const * data
		, uint32_t size )const
	{
		assert( size < getBuffer().getSize() );
		auto buffer = static_cast< BufferBase const & >( getBuffer() ).lock( 0
			, size
			, renderer::MemoryMapFlag::eWrite | renderer::MemoryMapFlag::eInvalidateRange );

		if ( !buffer )
		{
			throw std::runtime_error{ "Staging buffer storage memory mapping failed." };
		}

		std::memcpy( buffer
			, data
			, size );
		static_cast< BufferBase const & >( getBuffer() ).unlock( size, true );
	}

	void StagingBuffer::copyTextureData( renderer::CommandBuffer const & commandBuffer
		, vk::ByteArray const & data
		, renderer::Texture const & texture )const
	{
		doCopyToStagingBuffer( data.data()
			, uint32_t( data.size() ) );

		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eTopOfPipe
				, renderer::PipelineStageFlag::eTransfer
				, texture.makeTransferDestination() );
			commandBuffer.copyImage( *this
				, texture );
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eTransfer
				, renderer::PipelineStageFlag::eFragmentShader
				, texture.makeShaderInputResource() );
			bool res = commandBuffer.end();

			if ( !res )
			{
				throw std::runtime_error{ "Texture data copy failed." };
			}

			res = m_device.getGraphicsQueue().submit( commandBuffer
				, nullptr );

			if ( !res )
			{
				throw std::runtime_error{ "Texture data copy failed." };
			}

			m_device.getGraphicsQueue().waitIdle();
		}
	}

	void StagingBuffer::doCopyFromStagingBuffer( renderer::CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, renderer::BufferBase const & buffer )const
	{
		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			commandBuffer.copyBuffer( getBuffer()
				, buffer
				, size
				, offset );
			bool res = commandBuffer.end();

			if ( !res )
			{
				throw std::runtime_error{ "Buffer data copy failed." };
			}

			res = m_device.getGraphicsQueue().submit( commandBuffer
				, nullptr );

			if ( !res )
			{
				throw std::runtime_error{ "Texture data copy failed." };
			}

			m_device.getGraphicsQueue().waitIdle();
		}
	}

	void StagingBuffer::doCopyFromStagingBuffer( renderer::CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, renderer::VertexBufferBase const & buffer
		, renderer::PipelineStageFlags const & flags )const
	{
		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			commandBuffer.copyBuffer( getBuffer()
				, buffer.getBuffer()
				, size
				, offset );
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eTransfer
				, flags
				, buffer.getBuffer().makeVertexShaderInputResource() );
			bool res = commandBuffer.end();

			if ( !res )
			{
				throw std::runtime_error{ "Buffer data copy failed." };
			}

			res = m_device.getGraphicsQueue().submit( commandBuffer
				, nullptr );

			if ( !res )
			{
				throw std::runtime_error{ "Buffer data copy failed." };
			}

			m_device.getGraphicsQueue().waitIdle();
		}
	}

	void StagingBuffer::doCopyFromStagingBuffer( renderer::CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, renderer::UniformBufferBase const & buffer
		, renderer::PipelineStageFlags const & flags )const
	{
		if ( commandBuffer.begin( renderer::CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			commandBuffer.copyBuffer( getBuffer()
				, buffer.getBuffer()
				, size
				, offset );
			commandBuffer.memoryBarrier( renderer::PipelineStageFlag::eTransfer
				, flags
				, buffer.getBuffer().makeUniformBufferInput() );
			bool res = commandBuffer.end();

			if ( !res )
			{
				throw std::runtime_error{ "Buffer data copy failed." };
			}

			res = m_device.getGraphicsQueue().submit( commandBuffer
				, nullptr );

			if ( !res )
			{
				throw std::runtime_error{ "Buffer data copy failed." };
			}

			m_device.getGraphicsQueue().waitIdle();
		}
	}
}
