/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Buffer/StagingBuffer.hpp"

#include "Ashes/Buffer/Buffer.hpp"
#include "Ashes/Command/CommandBuffer.hpp"
#include "Ashes/Core/Device.hpp"
#include "Ashes/Core/Exception.hpp"
#include "Ashes/Image/Texture.hpp"
#include "Ashes/Image/TextureView.hpp"
#include "Ashes/Sync/BufferMemoryBarrier.hpp"
#include "Ashes/Sync/ImageMemoryBarrier.hpp"

namespace ashes
{
	namespace
	{
		uint32_t deduceMemoryType( uint32_t typeBits
			, ashes::MemoryPropertyFlags requirements
			, ashes::PhysicalDeviceMemoryProperties const & memoryProperties )
		{
			uint32_t result = 0xFFFFFFFFu;
			bool found{ false };

			// Recherche parmi les types de mémoire la première ayant les propriétés voulues.
			uint32_t i{ 0 };

			while ( i < memoryProperties.memoryTypes.size() && !found )
			{
				if ( ( typeBits & 1 ) == 1 )
				{
					// Le type de mémoire est disponible, a-t-il les propriétés demandées?
					if ( ( memoryProperties.memoryTypes[i].propertyFlags & requirements ) == requirements )
					{
						result = i;
						found = true;
					}
				}

				typeBits >>= 1;
				++i;
			}

			if ( !found )
			{
				throw ashes::Exception{ ashes::Result::eErrorRenderer, "Could not deduce memory type" };
			}

			return result;
		}

		ashes::MemoryAllocateInfo getAllocateInfo( Device const & device
			, BufferBase const & buffer )
		{
			auto requirements = buffer.getMemoryRequirements();
			auto deduced = deduceMemoryType( requirements.memoryTypeBits
				, ashes::MemoryPropertyFlag::eHostVisible
				, device.getMemoryProperties() );
			return { requirements.size, deduced };
		}
	}

	StagingBuffer::StagingBuffer( Device const & device
		, BufferTargets target
		, uint32_t size )
		: m_device{ device }
		, m_buffer{ device.createBuffer( size
			, target | BufferTarget::eTransferSrc | BufferTarget::eTransferDst ) }
		, m_storage{ device.allocateMemory( getAllocateInfo( device, *m_buffer ) ) }
	{
		m_buffer->bindMemory( m_storage );
	}

	void StagingBuffer::doCopyToStagingBuffer( uint8_t const * data
		, uint32_t size )const
	{
		assert( size <= getBuffer().getSize() );
		auto mappedSize = getAlignedSize( size
			, uint32_t( m_device.getProperties().limits.nonCoherentAtomSize ) );
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
			, uint32_t( m_device.getProperties().limits.nonCoherentAtomSize ) );
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
