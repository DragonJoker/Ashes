/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Buffer/StagingBuffer.hpp"

#include "Ashes/Command/CommandBuffer.hpp"
#include "Ashes/Core/Device.hpp"
#include "Ashes/Core/Exception.hpp"
#include "Ashes/Image/Texture.hpp"
#include "Ashes/Image/TextureView.hpp"
#include "Ashes/Sync/BufferMemoryBarrier.hpp"
#include "Ashes/Sync/ImageMemoryBarrier.hpp"

namespace ashes
{
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
		auto mappedSize = getAlignedSize( size
			, uint32_t( m_device.getPhysicalDevice().getProperties().limits.nonCoherentAtomSize ) );
		mappedSize = mappedSize > getBuffer().getSize()
			? ~( 0ull )
			: mappedSize;
		auto buffer = static_cast< BufferBase const & >( getBuffer() ).lock( 0u
			, mappedSize
			, MemoryMapFlag::eWrite | MemoryMapFlag::eInvalidateRange );

		if ( !buffer )
		{
			throw Exception{ Result::eErrorMemoryMapFailed, "Staging buffer storage memory mapping" };
		}

		std::memcpy( buffer
			, data
			, size );
		getBuffer().flush( 0u, mappedSize );
		getBuffer().unlock();
		m_device.waitIdle();
	}

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, BufferBase const & buffer )const
	{
		assert( size <= getBuffer().getSize() );
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
	}

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, VertexBufferBase const & buffer )const
	{
		assert( size <= getBuffer().getSize() );
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
	}

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, UniformBufferBase const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
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
	}

	void StagingBuffer::doCopyFromStagingBuffer( uint8_t * data
		, uint32_t size )const
	{
		assert( size <= getBuffer().getSize() );
		auto mappedSize = getAlignedSize( size
			, uint32_t( m_device.getPhysicalDevice().getProperties().limits.nonCoherentAtomSize ) );
		mappedSize = mappedSize > getBuffer().getSize()
			? ~( 0ull )
			: mappedSize;
		auto buffer = static_cast< BufferBase const & >( getBuffer() ).lock( 0u
			, mappedSize
			, MemoryMapFlag::eRead );

		if ( !buffer )
		{
			throw Exception{ Result::eErrorMemoryMapFailed, "Staging buffer storage memory mapping failed." };
		}

		std::memcpy( data
			, buffer
			, size );
		getBuffer().flush( 0u, mappedSize );
		getBuffer().unlock();
		m_device.waitIdle();
	}

	void StagingBuffer::doCopyToStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, BufferBase const & buffer )const
	{
		assert( size <= getBuffer().getSize() );
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
	}

	void StagingBuffer::doCopyToStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, VertexBufferBase const & buffer )const
	{
		assert( size <= getBuffer().getSize() );
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
	}

	void StagingBuffer::doCopyToStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, UniformBufferBase const & buffer
		, PipelineStageFlags dstStageFlags )const
	{
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
	}
}
