/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11MemoryBarrierCommand.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	MemoryBarrierCommand::MemoryBarrierCommand( VkDevice device
		, VkPipelineStageFlags after
		, VkPipelineStageFlags before
		, VkMemoryBarrierArray const & memoryBarriers
		, VkBufferMemoryBarrierArray const & bufferBarriers
		, VkImageMemoryBarrierArray const & imageBarriers )
		: CommandBase{ device }
	{
		for ( auto & barrier : bufferBarriers )
		{
			if ( get( barrier.buffer )->isMapped() )
			{
				if ( checkFlag( barrier.srcAccessMask, VK_ACCESS_MEMORY_WRITE_BIT )
					|| checkFlag( barrier.srcAccessMask, VK_ACCESS_HOST_WRITE_BIT )
					|| checkFlag( barrier.srcAccessMask, VK_ACCESS_TRANSFER_WRITE_BIT ) )
				{
					m_uploadBuffers.push_back(
						{
							barrier.offset,
							barrier.size,
							barrier.buffer,
						} );
				}
				else if ( checkFlag( barrier.dstAccessMask, VK_ACCESS_TRANSFER_READ_BIT )
					|| checkFlag( barrier.dstAccessMask, VK_ACCESS_HOST_READ_BIT )
					|| checkFlag( barrier.dstAccessMask, VK_ACCESS_MEMORY_READ_BIT ) )
				{
					m_downloadBuffers.push_back(
						{
							barrier.offset,
							barrier.size,
							barrier.buffer,
						} );
				}
			}
		}
	}

	void MemoryBarrierCommand::apply( Context const & context )const
	{
		for ( auto & buffer : m_uploadBuffers )
		{
			get( get( buffer.buffer )->getMemory() )->updateUpload( buffer.offset
				, buffer.size );
		}

		for ( auto & buffer : m_uploadBuffers )
		{
			get( get( buffer.buffer )->getMemory() )->updateDownload( buffer.offset
				, buffer.size );
		}
	}

	CommandPtr MemoryBarrierCommand::clone()const
	{
		return std::make_unique< MemoryBarrierCommand >( *this );
	}
}
