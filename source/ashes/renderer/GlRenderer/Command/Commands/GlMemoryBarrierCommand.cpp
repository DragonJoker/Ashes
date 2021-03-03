/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlMemoryBarrierCommand.hpp"

#include "Core/GlContextLock.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	void apply( ContextLock const & context
		, CmdMemoryBarrier const & cmd )
	{
		glLogCall( context
			, glMemoryBarrier
			, cmd.flags );
	}

	void buildMemoryBarrierCommand( VkPipelineStageFlags after
		, VkPipelineStageFlags before
		, VkDependencyFlags dependencyFlags
		, ArrayView< VkMemoryBarrier const > memoryBarriers
		, ArrayView< VkBufferMemoryBarrier const > bufferMemoryBarriers
		, ArrayView< VkImageMemoryBarrier const > imageMemoryBarriers
		, VkDeviceMemorySet & downloads
		, VkDeviceMemorySet & uploads
		, CmdList & list )
	{
		glLogCommand( list, "MemoryBarrierCommand" );
		bool hasMapped = false;

		for ( auto & barrier : bufferMemoryBarriers )
		{
			if ( get( barrier.buffer )->isMapped() )
			{
				hasMapped = true;
				if ( !checkFlag( barrier.dstAccessMask, VK_ACCESS_TRANSFER_READ_BIT )
					&& ( checkFlag( barrier.srcAccessMask, VK_ACCESS_MEMORY_WRITE_BIT )
						|| checkFlag( barrier.srcAccessMask, VK_ACCESS_HOST_WRITE_BIT ) ) )
				{
					uploads.insert( get( barrier.buffer )->getMemoryBinding().getParent() );
					list.push_back( makeCmd< OpType::eUploadMemory >( get( barrier.buffer )->getMemoryBinding().getParent() ) );
				}
				else if ( checkFlag( barrier.srcAccessMask, VK_ACCESS_TRANSFER_WRITE_BIT ) )
				{
					downloads.insert( get( barrier.buffer )->getMemoryBinding().getParent() );
					list.push_back( makeCmd< OpType::eDownloadMemory >( get( barrier.buffer )->getMemoryBinding().getParent() ) );
				}
			}
		}

		list.push_back( makeCmd< OpType::eMemoryBarrier >( getMemoryBarrierFlags( before )
			| ( hasMapped
				? GL_MEMORY_BARRIER_CLIENT_MAPPED_BUFFER
				: 0u ) ) );

		for ( auto & barrier : bufferMemoryBarriers )
		{
			if ( get( barrier.buffer )->isMapped() )
			{
				if ( !checkFlag( barrier.srcAccessMask, VK_ACCESS_TRANSFER_WRITE_BIT )
					&& ( checkFlag( barrier.dstAccessMask, VK_ACCESS_HOST_READ_BIT )
						|| checkFlag( barrier.dstAccessMask, VK_ACCESS_MEMORY_READ_BIT ) ) )
				{
					downloads.insert( get( barrier.buffer )->getMemoryBinding().getParent() );
					list.push_back( makeCmd< OpType::eDownloadMemory >( get( barrier.buffer )->getMemoryBinding().getParent() ) );
				}
				else if ( checkFlag( barrier.dstAccessMask, VK_ACCESS_TRANSFER_READ_BIT ) )
				{
					uploads.insert( get( barrier.buffer )->getMemoryBinding().getParent() );
					list.push_back( makeCmd< OpType::eUploadMemory >( get( barrier.buffer )->getMemoryBinding().getParent() ) );
				}
			}
		}
	}
}
