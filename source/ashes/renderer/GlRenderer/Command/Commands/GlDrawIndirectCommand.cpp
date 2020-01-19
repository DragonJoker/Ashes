/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlDrawIndirectCommand.hpp"

#include "Buffer/GlBuffer.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	void apply( ContextLock const & context
		, CmdDrawIndirect const & cmd )
	{
		glLogCall( context
			, glMultiDrawArraysIndirect
			, cmd.mode
			, getBufferOffset( cmd.offset )
			, cmd.drawCount
			, cmd.stride );
	}

	void buildDrawIndirectCommand( VkBuffer buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride
		, VkPrimitiveTopology mode
		, CmdList & list )
	{
		glLogCommand( list, "DrawIndirectCommand" );
		list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_DRAW_INDIRECT
			, get( buffer )->getInternal() ) );
		list.push_back( makeCmd< OpType::eDrawIndirect >( uint32_t( get( buffer )->getInternalOffset() + offset )
			, drawCount
			, stride
			, convert( mode ) ) );
		list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_DRAW_INDIRECT
			, 0u ) );
	}
}
