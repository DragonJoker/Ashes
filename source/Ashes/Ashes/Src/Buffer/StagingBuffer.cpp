/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Buffer/StagingBuffer.hpp"

#include "Core/Device.hpp"
#include "Core/Exception.hpp"
#include "Image/Texture.hpp"
#include "Image/TextureView.hpp"
#include "Sync/BufferMemoryBarrier.hpp"
#include "Sync/ImageMemoryBarrier.hpp"

namespace ashes
{
	namespace
	{
		inline uint32_t getAlignedSize( uint32_t size, uint32_t align )
		{
			uint32_t result = 0u;

			while ( size > align )
			{
				size -= align;
				result += align;
			}

			return result + align;
		}
	}

	StagingBuffer::StagingBuffer( Device const & device
		, BufferTargets target
		, uint32_t size )
		: m_device{ device }
		, m_buffer{ device.createBuffer( size
			, target | BufferTarget::eTransferSrc | BufferTarget::eTransferDst
			, MemoryPropertyFlag::eHostVisible ) }
	{
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
		getBuffer().flush( 0u
			, std::min( getBuffer().getSize()
				, getAlignedSize( size
					, uint32_t( m_device.getPhysicalDevice().getProperties().limits.nonCoherentAtomSize ) ) ) );
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
			, ashes::PipelineStageFlag::eVertexInput
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
		getBuffer().flush( 0u
			, std::min( getBuffer().getSize()
				, getAlignedSize( size
					, uint32_t( m_device.getPhysicalDevice().getProperties().limits.nonCoherentAtomSize ) ) ) );
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
			, ashes::PipelineStageFlag::eVertexInput
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
