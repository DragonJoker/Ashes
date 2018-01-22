/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "Buffer/StagingBuffer.hpp"

#include "Image/Texture.hpp"
#include "Sync/BufferMemoryBarrier.hpp"
#include "Sync/ImageMemoryBarrier.hpp"

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
			, MemoryMapFlag::eWrite | MemoryMapFlag::eInvalidateRange );

		if ( !buffer )
		{
			throw std::runtime_error{ "Staging buffer storage memory mapping failed." };
		}

		std::memcpy( buffer
			, data
			, size );
		static_cast< BufferBase const & >( getBuffer() ).unlock( size, true );
	}

	void StagingBuffer::copyTextureData( CommandBuffer const & commandBuffer
		, ByteArray const & data
		, Texture const & texture )const
	{
		if ( data.size() < getBuffer().getSize() )
		{
			doCopyToStagingBuffer( data.data()
				, uint32_t( data.size() ) );

			if ( commandBuffer.begin( CommandBufferUsageFlag::eOneTimeSubmit ) )
			{
				commandBuffer.memoryBarrier( PipelineStageFlag::eTopOfPipe
					, PipelineStageFlag::eTransfer
					, texture.makeTransferDestination() );
				commandBuffer.copyToImage( BufferImageCopy
					{
						0u,
						0u,
						0u,
						{
							getAspectMask( texture.getFormat() ),
							0u,
							0u,
							1u,
						},
						{
							0,
							0,
							0
						},
						{
							texture.getDimensions()[0],
							std::max( 1u, texture.getDimensions()[1] ),
							std::max( 1u, texture.getDimensions()[2] )
						}
					}
					, getBuffer()
					, texture );
				commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
					, PipelineStageFlag::eFragmentShader
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
		else
		{
			size_t remaining = data.size();
			size_t offset = 0u;

			while ( remaining > data.size() )
			{
				doCopyToStagingBuffer( data.data() + offset
					, uint32_t( getBuffer().getSize() ) );

				if ( commandBuffer.begin( CommandBufferUsageFlag::eOneTimeSubmit ) )
				{
					commandBuffer.memoryBarrier( PipelineStageFlag::eTopOfPipe
						, PipelineStageFlag::eTransfer
						, texture.makeTransferDestination() );
					commandBuffer.copyToImage( BufferImageCopy
						{
							0u,
							0u,
							0u,
							{
								getAspectMask( texture.getFormat() ),
								0u,
								0u,
								1u,
							},
							{
								0,
								0,
								0
							},
							{
								texture.getDimensions()[0],
								std::max( 1u, texture.getDimensions()[1] ),
								std::max( 1u, texture.getDimensions()[2] )
							}
						}
						, getBuffer()
						, texture );
					commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
						, PipelineStageFlag::eFragmentShader
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
		}
	}

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, BufferBase const & buffer )const
	{
		if ( commandBuffer.begin( CommandBufferUsageFlag::eOneTimeSubmit ) )
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

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, VertexBufferBase const & buffer
		, PipelineStageFlags const & flags )const
	{
		if ( commandBuffer.begin( CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			commandBuffer.copyBuffer( getBuffer()
				, buffer.getBuffer()
				, size
				, offset );
			commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
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

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, UniformBufferBase const & buffer
		, PipelineStageFlags const & flags )const
	{
		if ( commandBuffer.begin( CommandBufferUsageFlag::eOneTimeSubmit ) )
		{
			commandBuffer.copyBuffer( getBuffer()
				, buffer.getBuffer()
				, size
				, offset );
			commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
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
