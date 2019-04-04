/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/Buffer/StagingBuffer.hpp"

#include "AshesPP/Buffer/Buffer.hpp"
#include "AshesPP/Command/CommandBuffer.hpp"
#include "AshesPP/Core/Device.hpp"
#include "AshesPP/Image/Image.hpp"
#include "AshesPP/Image/ImageView.hpp"

#include <AshesRenderer/Util/Exception.hpp>

namespace ashes
{
	namespace
	{
		uint32_t deduceMemoryType( uint32_t typeBits
			, VkMemoryPropertyFlags requirements
			, VkPhysicalDeviceMemoryProperties const & memoryProperties )
		{
			uint32_t result = 0xFFFFFFFFu;
			bool found{ false };

			// Recherche parmi les types de m�moire la premi�re ayant les propri�t�s voulues.
			uint32_t i{ 0 };

			while ( i < memoryProperties.memoryTypeCount && !found )
			{
				if ( ( typeBits & 1 ) == 1 )
				{
					// Le type de m�moire est disponible, a-t-il les propri�t�s demand�es?
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
				throw Exception{ VkResult::VK_ERROR_VALIDATION_FAILED_EXT, "Could not deduce memory type" };
			}

			return result;
		}

		VkMemoryAllocateInfo getAllocateInfo( Device const & device
			, BufferBase const & buffer )
		{
			auto requirements = buffer.getMemoryRequirements();
			auto deduced = deduceMemoryType( requirements.memoryTypeBits
				, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
				, device.getMemoryProperties() );
			return
			{
				VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				nullptr,
				requirements.size,
				deduced
			};
		}
	}

	StagingBuffer::StagingBuffer( Device const & device
		, VkBufferUsageFlags target
		, uint32_t size )
		: m_device{ device }
		, m_buffer{ device.createBuffer( size
			, target | VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_DST_BIT ) }
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
			, 0u );

		if ( !buffer )
		{
			throw Exception{ VkResult::VK_ERROR_MEMORY_MAP_FAILED, "Staging buffer storage memory mapping" };
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
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, getBuffer().makeTransferSource() );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
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
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, getBuffer().makeTransferSource() );
		auto srcStageFlags = buffer.getBuffer().getCompatibleStageFlags();
		commandBuffer.memoryBarrier( srcStageFlags
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, buffer.getBuffer().makeTransferDestination() );
		commandBuffer.copyBuffer( getBuffer()
			, buffer.getBuffer()
			, size
			, offset );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
			, buffer.getBuffer().makeVertexShaderInputResource() );
	}

	void StagingBuffer::doCopyFromStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, UniformBufferBase const & buffer
		, VkPipelineStageFlags dstStageFlags )const
	{
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, getBuffer().makeTransferSource() );
		auto srcStageFlags = buffer.getBuffer().getCompatibleStageFlags();
		commandBuffer.memoryBarrier( srcStageFlags
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, buffer.getBuffer().makeTransferDestination() );
		commandBuffer.copyBuffer( getBuffer()
			, buffer.getBuffer()
			, size
			, offset );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
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
			, 0u );

		if ( !buffer )
		{
			throw Exception{ VkResult::VK_ERROR_MEMORY_MAP_FAILED, "Staging buffer storage memory mapping failed." };
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
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, buffer.makeTransferSource() );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
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
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, buffer.getBuffer().makeTransferSource() );
		auto srcStageFlags = buffer.getBuffer().getCompatibleStageFlags();
		commandBuffer.memoryBarrier( srcStageFlags
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, getBuffer().makeTransferDestination() );
		commandBuffer.copyBuffer( buffer.getBuffer()
			, getBuffer()
			, size
			, offset );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT
			, buffer.getBuffer().makeVertexShaderInputResource() );
	}

	void StagingBuffer::doCopyToStagingBuffer( CommandBuffer const & commandBuffer
		, uint32_t size
		, uint32_t offset
		, UniformBufferBase const & buffer
		, VkPipelineStageFlags dstStageFlags )const
	{
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, buffer.getBuffer().makeTransferSource() );
		auto srcStageFlags = buffer.getBuffer().getCompatibleStageFlags();
		commandBuffer.memoryBarrier( srcStageFlags
			, VK_PIPELINE_STAGE_TRANSFER_BIT
			, getBuffer().makeTransferDestination() );
		commandBuffer.copyBuffer( buffer.getBuffer()
			, getBuffer()
			, size
			, offset );
		commandBuffer.memoryBarrier( VK_PIPELINE_STAGE_TRANSFER_BIT
			, dstStageFlags
			, buffer.getBuffer().makeUniformBufferInput() );
	}
}
