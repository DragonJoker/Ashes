/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlDrawIndexedIndirectCommand.hpp"

#include "Buffer/GlBuffer.hpp"

#include "ashesgl3_api.hpp"

namespace ashes::gl3
{
	void apply( ContextLock const & context
		, CmdDrawIndexedIndirect const & cmd )
	{
		glLogCall( context
			, glMultiDrawElementsIndirect_ARB
			, cmd.mode
			, cmd.type
			, getBufferOffset( cmd.offset )
			, cmd.drawCount
			, cmd.stride );
	}

	void buildDrawIndexedIndirectCommand( VkBuffer buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride
		, VkPrimitiveTopology mode
		, VkIndexType type
		, CmdList & list )
	{
		glLogCommand( "DrawIndexedIndirectCommand" );
		list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_DRAW_INDIRECT
			, get( buffer )->getInternal() ) );
		list.push_back( makeCmd< OpType::eDrawIndexedIndirect >( uint32_t( get( buffer )->getInternalOffset() + offset )
			, drawCount
			, stride
			, convert( mode )
			, convert( type ) ) );
		list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_DRAW_INDIRECT
			, 0u ) );
	}
}
