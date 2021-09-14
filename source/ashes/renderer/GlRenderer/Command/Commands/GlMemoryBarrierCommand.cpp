/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlMemoryBarrierCommand.hpp"

#include "Core/GlContextLock.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	namespace
	{
		bool bindPreBarrier( DeviceMemoryBinding const & binding
			, VkAccessFlags srcAccessMask
			, VkAccessFlags dstAccessMask
			, VkDeviceMemorySet & downloads
			, VkDeviceMemorySet & uploads
			, CmdList & list )
		{
			bool result = binding.isMapped();

			if ( result )
			{
				if ( !checkFlag( dstAccessMask, VK_ACCESS_TRANSFER_READ_BIT )
					&& ( checkFlag( srcAccessMask, VK_ACCESS_MEMORY_WRITE_BIT )
						|| checkFlag( srcAccessMask, VK_ACCESS_HOST_WRITE_BIT ) ) )
				{
					uploads.insert( binding.getParent() );
					list.push_back( makeCmd< OpType::eUploadMemory >( binding.getParent()
						, binding.getOffset()
						, binding.getSize() ) );
				}
				else if ( checkFlag( srcAccessMask, VK_ACCESS_TRANSFER_WRITE_BIT ) )
				{
					downloads.insert( binding.getParent() );
					list.push_back( makeCmd< OpType::eDownloadMemory >( binding.getParent()
						, binding.getOffset()
						, binding.getSize() ) );
				}
			}

			return result;
		}

		bool bindPostBarrier( DeviceMemoryBinding const & binding
			, VkAccessFlags srcAccessMask
			, VkAccessFlags dstAccessMask
			, VkDeviceMemorySet & downloads
			, VkDeviceMemorySet & uploads
			, CmdList & list )
		{
			bool result = binding.isMapped();

			if ( binding.isMapped() )
			{
				if ( !checkFlag( srcAccessMask, VK_ACCESS_TRANSFER_WRITE_BIT )
					&& ( checkFlag( dstAccessMask, VK_ACCESS_HOST_READ_BIT )
						|| checkFlag( dstAccessMask, VK_ACCESS_MEMORY_READ_BIT ) ) )
				{
					downloads.insert( binding.getParent() );
					list.push_back( makeCmd< OpType::eDownloadMemory >( binding.getParent()
						, binding.getOffset()
						, binding.getSize() ) );
				}
				else if ( checkFlag( dstAccessMask, VK_ACCESS_TRANSFER_READ_BIT ) )
				{
					uploads.insert( binding.getParent() );
					list.push_back( makeCmd< OpType::eUploadMemory >( binding.getParent()
						, binding.getOffset()
						, binding.getSize() ) );
				}
			}

			return result;
		}
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
			hasMapped = bindPreBarrier( get( barrier.buffer )->getMemoryBinding()
				, barrier.srcAccessMask
				, barrier.dstAccessMask
				, downloads 
				, uploads
				, list ) || hasMapped;
		}

		for ( auto & barrier : imageMemoryBarriers )
		{
			hasMapped = bindPreBarrier( get( barrier.image )->getMemoryBinding()
				, barrier.srcAccessMask
				, barrier.dstAccessMask
				, downloads
				, uploads
				, list ) || hasMapped;
		}

		list.push_back( makeCmd< OpType::eMemoryBarrier >( getMemoryBarrierFlags( before )
			| ( hasMapped
				? GL_MEMORY_BARRIER_CLIENT_MAPPED_BUFFER
				: 0u ) ) );

		for ( auto & barrier : bufferMemoryBarriers )
		{
			bindPostBarrier( get( barrier.buffer )->getMemoryBinding()
				, barrier.srcAccessMask
				, barrier.dstAccessMask
				, downloads
				, uploads
				, list );
		}

		for ( auto & barrier : imageMemoryBarriers )
		{
			bindPostBarrier( get( barrier.image )->getMemoryBinding()
				, barrier.srcAccessMask
				, barrier.dstAccessMask
				, downloads
				, uploads
				, list );
		}
	}
}
