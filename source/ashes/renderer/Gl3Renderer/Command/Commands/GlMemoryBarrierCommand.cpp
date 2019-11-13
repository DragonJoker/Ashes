/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlMemoryBarrierCommand.hpp"

#include "Core/GlContextLock.hpp"

#include "ashesgl3_api.hpp"

namespace ashes::gl3
{
	void apply( ContextLock const & context
		, CmdMemoryBarrier const & cmd )
	{
		glLogCall( context
			, glMemoryBarrier_ARB
			, cmd.flags );
	}

	void buildMemoryBarrierCommand( VkPipelineStageFlags after
		, VkPipelineStageFlags before
		, VkDependencyFlags dependencyFlags
		, VkMemoryBarrierArray memoryBarriers
		, VkBufferMemoryBarrierArray bufferMemoryBarriers
		, VkImageMemoryBarrierArray imageMemoryBarriers
		, CmdList & list )
	{
		glLogCommand( "MemoryBarrierCommand" );

		for ( auto & barrier : bufferMemoryBarriers )
		{
			if ( get( barrier.buffer )->isMapped() )
			{
				if ( checkFlag( barrier.srcAccessMask, VK_ACCESS_MEMORY_WRITE_BIT )
					|| checkFlag( barrier.srcAccessMask, VK_ACCESS_HOST_WRITE_BIT )
					|| checkFlag( barrier.srcAccessMask, VK_ACCESS_TRANSFER_WRITE_BIT ) )
				{
					list.push_back( makeCmd< OpType::eUploadMemory >( get( barrier.buffer )->getMemory() ) );
				}
			}
		}

		list.push_back( makeCmd< OpType::eMemoryBarrier >( getMemoryBarrierFlags( before ) ) );

		for ( auto & barrier : bufferMemoryBarriers )
		{
			if ( get( barrier.buffer )->isMapped() )
			{
				if ( checkFlag( barrier.dstAccessMask, VK_ACCESS_TRANSFER_READ_BIT )
					|| checkFlag( barrier.dstAccessMask, VK_ACCESS_HOST_READ_BIT )
					|| checkFlag( barrier.dstAccessMask, VK_ACCESS_MEMORY_READ_BIT ) )
				{
					list.push_back( makeCmd< OpType::eDownloadMemory >( get( barrier.buffer )->getMemory() ) );
				}
			}
		}
	}
}
