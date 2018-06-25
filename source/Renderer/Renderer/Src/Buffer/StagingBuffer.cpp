/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Buffer/StagingBuffer.hpp"

#include "Core/Device.hpp"
#include "Core/Exception.hpp"
#include "Image/Texture.hpp"
#include "Image/TextureView.hpp"
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

	void StagingBuffer::uploadTextureData( CommandBuffer const & commandBuffer
		, ImageSubresourceLayers const & subresourceLayers
		, Offset3D const & offset
		, Extent3D const & extent
		, uint8_t const * const data
		, uint32_t size
		, TextureView const & view )const
	{
		assert( size <= getBuffer().getSize() );
		doCopyToStagingBuffer( data, size );

		commandBuffer.begin( CommandBufferUsageFlag::eOneTimeSubmit );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTopOfPipe
			, PipelineStageFlag::eTransfer
			, view.makeTransferDestination( ImageLayout::eUndefined
				, 0u ) );
		commandBuffer.copyToImage( BufferImageCopy
			{
				0u,
				0u,
				0u,
				subresourceLayers,
				offset,
				Extent3D{
					std::max( 1u, extent.width ),
					std::max( 1u, extent.height ),
					std::max( 1u, extent.depth )
				}
			}
			, getBuffer()
			, view.getTexture() );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, PipelineStageFlag::eFragmentShader
			, view.makeShaderInputResource( ImageLayout::eTransferDstOptimal
				, renderer::AccessFlag::eTransferWrite ) );
		commandBuffer.end();

		auto fence = m_device.createFence();
		m_device.getGraphicsQueue().submit( commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );
	}

	void StagingBuffer::uploadTextureData( CommandBuffer const & commandBuffer
		, uint8_t const * const data
		, uint32_t size
		, TextureView const & view )const
	{
		uploadTextureData( commandBuffer
			, {
				getAspectMask( view.getFormat() ),
				view.getSubResourceRange().baseMipLevel,
				view.getSubResourceRange().baseArrayLayer,
				view.getSubResourceRange().layerCount
			}
			, Offset3D{ 0, 0, 0 }
			, view.getTexture().getDimensions()
			, data
			, size
			, view );
	}

	void StagingBuffer::downloadTextureData( CommandBuffer const & commandBuffer
		, ImageSubresourceLayers const & subresourceLayers
		, Offset3D const & offset
		, Extent3D const & extent
		, uint8_t * data
		, uint32_t size
		, TextureView const & view )const
	{
		assert( size <= getBuffer().getSize() );
		commandBuffer.begin( CommandBufferUsageFlag::eOneTimeSubmit );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTopOfPipe
			, PipelineStageFlag::eTransfer
			, view.makeTransferSource( ImageLayout::eUndefined
				, 0u ) );
		commandBuffer.copyToBuffer( BufferImageCopy
			{
				0u,
				0u,
				0u,
				subresourceLayers,
				offset,
				Extent3D{
					std::max( 1u, extent.width ),
					std::max( 1u, extent.height ),
					std::max( 1u, extent.depth )
				}
			}
			, view.getTexture()
			, getBuffer() );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, PipelineStageFlag::eFragmentShader
			, view.makeShaderInputResource( ImageLayout::eTransferSrcOptimal
			, renderer::AccessFlag::eTransferRead ) );
		commandBuffer.end();

		auto fence = m_device.createFence();
		m_device.getGraphicsQueue().submit( commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );

		doCopyFromStagingBuffer( data, size );
	}

	void StagingBuffer::downloadTextureData( CommandBuffer const & commandBuffer
		, uint8_t * data
		, uint32_t size
		, TextureView const & view )const
	{
		downloadTextureData( commandBuffer
			, {
				getAspectMask( view.getFormat() ),
				view.getSubResourceRange().baseMipLevel,
				view.getSubResourceRange().baseArrayLayer,
				view.getSubResourceRange().layerCount
			}
			, Offset3D{ 0, 0, 0 }
			, view.getTexture().getDimensions()
			, data
			, size
			, view );
	}

	void StagingBuffer::doCopyToStagingBuffer( uint8_t const * data
		, uint32_t size )const
	{
		assert( size <= getBuffer().getSize() );
		auto buffer = static_cast< BufferBase const & >( getBuffer() ).lock( 0u
			, size
			, MemoryMapFlag::eWrite | MemoryMapFlag::eInvalidateRange );

		if ( !buffer )
		{
			throw Exception{ Result::eErrorMemoryMapFailed, "Staging buffer storage memory mapping" };
		}

		std::memcpy( buffer
			, data
			, size );
		getBuffer().flush( 0u, size );
		getBuffer().unlock();
		m_device.waitIdle();
	}

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, BufferBase const & buffer )const
	{
		assert( size <= getBuffer().getSize() );
		commandBuffer.begin( CommandBufferUsageFlag::eOneTimeSubmit );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, PipelineStageFlag::eTransfer
			, getBuffer().makeTransferSource() );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, PipelineStageFlag::eTransfer
			, buffer.makeTransferDestination() );
		commandBuffer.copyBuffer( getBuffer()
			, buffer
			, size
			, offset );
		commandBuffer.end();

		auto fence = m_device.createFence();
		m_device.getGraphicsQueue().submit( commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );
	}

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, VertexBufferBase const & buffer )const
	{
		assert( size <= getBuffer().getSize() );
		commandBuffer.begin( CommandBufferUsageFlag::eOneTimeSubmit );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, PipelineStageFlag::eTransfer
			, getBuffer().makeTransferSource() );
		auto srcStageFlags = buffer.getBuffer().getCompatibleStageFlags();
		commandBuffer.memoryBarrier( srcStageFlags
			, PipelineStageFlag::eTransfer
			, buffer.getBuffer().makeTransferDestination() );
		commandBuffer.copyBuffer( getBuffer()
			, buffer.getBuffer()
			, size
			, offset );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, renderer::PipelineStageFlag::eVertexInput
			, buffer.getBuffer().makeVertexShaderInputResource() );
		commandBuffer.end();

		auto fence = m_device.createFence();
		m_device.getGraphicsQueue().submit( commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );
	}

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, UniformBufferBase const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
		commandBuffer.begin( CommandBufferUsageFlag::eOneTimeSubmit );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, PipelineStageFlag::eTransfer
			, getBuffer().makeTransferSource() );
		auto srcStageFlags = buffer.getBuffer().getCompatibleStageFlags();
		commandBuffer.memoryBarrier( srcStageFlags
			, PipelineStageFlag::eTransfer
			, buffer.getBuffer().makeTransferDestination() );
		commandBuffer.copyBuffer( getBuffer()
			, buffer.getBuffer()
			, size
			, offset );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, dstStageFlags
			, buffer.getBuffer().makeUniformBufferInput() );
		commandBuffer.end();

		auto fence = m_device.createFence();
		m_device.getGraphicsQueue().submit( commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );
	}

	void StagingBuffer::doCopyFromStagingBuffer( uint8_t * data
		, uint32_t size )const
	{
		assert( size <= getBuffer().getSize() );
		auto buffer = static_cast< BufferBase const & >( getBuffer() ).lock( 0u
			, size
			, MemoryMapFlag::eRead );

		if ( !buffer )
		{
			throw Exception{ Result::eErrorMemoryMapFailed, "Staging buffer storage memory mapping failed." };
		}

		std::memcpy( data
			, buffer
			, size );
		getBuffer().flush( 0u, size );
		getBuffer().unlock();
		m_device.waitIdle();
	}

	void StagingBuffer::doCopyToStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, BufferBase const & buffer )const
	{
		assert( size <= getBuffer().getSize() );
		commandBuffer.begin( CommandBufferUsageFlag::eOneTimeSubmit );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, PipelineStageFlag::eTransfer
			, buffer.makeTransferSource() );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, PipelineStageFlag::eTransfer
			, getBuffer().makeTransferDestination() );
		commandBuffer.copyBuffer( buffer
			, getBuffer()
			, size
			, offset );
		commandBuffer.end();

		auto fence = m_device.createFence();
		m_device.getGraphicsQueue().submit( commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );
	}

	void StagingBuffer::doCopyToStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, VertexBufferBase const & buffer )const
	{
		assert( size <= getBuffer().getSize() );
		commandBuffer.begin( CommandBufferUsageFlag::eOneTimeSubmit );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, PipelineStageFlag::eTransfer
			, buffer.getBuffer().makeTransferSource() );
		auto srcStageFlags = buffer.getBuffer().getCompatibleStageFlags();
		commandBuffer.memoryBarrier( srcStageFlags
			, PipelineStageFlag::eTransfer
			, getBuffer().makeTransferDestination() );
		commandBuffer.copyBuffer( buffer.getBuffer()
			, getBuffer()
			, size
			, offset );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, renderer::PipelineStageFlag::eVertexInput
			, buffer.getBuffer().makeVertexShaderInputResource() );
		commandBuffer.end();

		auto fence = m_device.createFence();
		m_device.getGraphicsQueue().submit( commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );
	}

	void StagingBuffer::doCopyToStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, UniformBufferBase const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
		commandBuffer.begin( CommandBufferUsageFlag::eOneTimeSubmit );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, PipelineStageFlag::eTransfer
			, buffer.getBuffer().makeTransferSource() );
		auto srcStageFlags = buffer.getBuffer().getCompatibleStageFlags();
		commandBuffer.memoryBarrier( srcStageFlags
			, PipelineStageFlag::eTransfer
			, getBuffer().makeTransferDestination() );
		commandBuffer.copyBuffer( buffer.getBuffer()
			, getBuffer()
			, size
			, offset );
		commandBuffer.memoryBarrier( PipelineStageFlag::eTransfer
			, dstStageFlags
			, buffer.getBuffer().makeUniformBufferInput() );
		commandBuffer.end();

		auto fence = m_device.createFence();
		m_device.getGraphicsQueue().submit( commandBuffer
			, fence.get() );
		fence->wait( FenceTimeout );
	}
}
