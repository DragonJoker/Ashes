/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlDispatchIndirectCommand.hpp"

#include "Buffer/GlBuffer.hpp"

#include "ashesgl3_api.hpp"

namespace ashes::gl3
{
	void apply( ContextLock const & context
		, CmdDispatchIndirect const & cmd )
	{
		glLogCall( context
			, glDispatchComputeIndirect_ARB
			, GLintptr( getBufferOffset( cmd.offset ) ) );
	}

	void buildDispatchIndirectCommand( VkBuffer buffer
		, VkDeviceSize offset
		, CmdList & list )
	{
		glLogCommand( "DispatchIndirectCommand" );
		list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_DISPATCH_INDIRECT
			, get( buffer )->getInternal() ) );
		list.push_back( makeCmd< OpType::eDispatchIndirect >( get( buffer )->getInternalOffset() + offset ) );
		list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_DISPATCH_INDIRECT
			, 0u ) );
	}
}
