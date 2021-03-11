/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlCopyBufferCommand.hpp"

#include "Buffer/GlBuffer.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	namespace
	{
		void adjustCopyInfo( DeviceMemoryBinding const & binding
			, VkDeviceSize & offset )
		{
			offset += binding.getOffset();
		}
	}

	void buildCopyBufferCommand( VkBufferCopy copyInfo
		, VkBuffer src
		, VkBuffer dst
		, CmdList & list )
	{
		glLogCommand( list, "CopyBufferCommand" );
		adjustCopyInfo( get( src )->getMemoryBinding(), copyInfo.srcOffset );
		adjustCopyInfo( get( dst )->getMemoryBinding(), copyInfo.dstOffset );

		if ( copyInfo.size == WholeSize )
		{
			copyInfo.size = std::min( get( src )->getMemoryBinding().getSize()
				, get( dst )->getMemoryBinding().getSize() );
		}

		list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_COPY_READ
			, get( src )->getInternal() ) );
		list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_COPY_WRITE
			, get( dst )->getInternal() ) );
		list.push_back( makeCmd< OpType::eCopyBufferSubData >( GL_BUFFER_TARGET_COPY_READ
			, GL_BUFFER_TARGET_COPY_WRITE
			, std::move( copyInfo ) ) );
		list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_COPY_WRITE
			, 0u ) );
		list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_COPY_READ
			, 0u ) );
	}
}
