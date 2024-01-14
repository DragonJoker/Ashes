/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestMemoryBarrierCommand.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	MemoryBarrierCommand::MemoryBarrierCommand( VkDevice device
		, VkPipelineStageFlags
		, VkPipelineStageFlags
		, VkMemoryBarrierArray const &
		, VkBufferMemoryBarrierArray const & bufferBarriers
		, VkImageMemoryBarrierArray const & )
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
					m_uploadBuffers.emplace_back( barrier.offset
						, barrier.size
						, barrier.buffer );
				}
				else if ( checkFlag( barrier.dstAccessMask, VK_ACCESS_TRANSFER_READ_BIT )
					|| checkFlag( barrier.dstAccessMask, VK_ACCESS_HOST_READ_BIT )
					|| checkFlag( barrier.dstAccessMask, VK_ACCESS_MEMORY_READ_BIT ) )
				{
					m_downloadBuffers.emplace_back( barrier.offset
						, barrier.size
						, barrier.buffer );
				}
			}
		}
	}

	void MemoryBarrierCommand::apply()const
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
