/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlCopyBufferCommand.hpp"

#include "Buffer/GlBuffer.hpp"

#include "ashesgl3_api.hpp"

namespace ashes::gl3
{
	void apply( ContextLock const & context
		, CmdCopyBufferSubData const & cmd )
	{
		glLogCall( context
			, glCopyBufferSubData
			, cmd.srcTarget
			, cmd.dstTarget
			, GLintptr( cmd.copy.srcOffset )
			, GLintptr( cmd.copy.dstOffset )
			, GLsizeiptr( cmd.copy.size ) );
	}

	void buildCopyBufferCommand( VkBufferCopy copyInfo
		, VkBuffer src
		, VkBuffer dst
		, CmdList & list )
	{
		glLogCommand( "CopyBufferCommand" );

		if ( get( src )->getTarget() == get( dst )->getTarget() )
		{
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_COPY_READ
				, get( src )->getInternal() ) );
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_COPY_WRITE
				, get( dst )->getInternal() ) );
			list.push_back( makeCmd< OpType::eCopyBufferSubData >( GL_BUFFER_TARGET_COPY_READ
				, GL_BUFFER_TARGET_COPY_WRITE
				, std::move( copyInfo ) ) );
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_COPY_READ
				, 0u ) );
			list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_COPY_WRITE
				, 0u ) );
		}
		else
		{
			list.push_back( makeCmd< OpType::eBindBuffer >( get( src )->getTarget()
				, get( src )->getInternal() ) );
			list.push_back( makeCmd< OpType::eBindBuffer >( get( dst )->getTarget()
				, get( dst )->getInternal() ) );
			list.push_back( makeCmd< OpType::eCopyBufferSubData >( get( src )->getTarget()
				, get( dst )->getTarget()
				, std::move( copyInfo ) ) );
			list.push_back( makeCmd< OpType::eBindBuffer >( get( src )->getTarget()
				, 0u ) );
			list.push_back( makeCmd< OpType::eBindBuffer >( get( dst )->getTarget()
				, 0u ) );
		}
	}
}
