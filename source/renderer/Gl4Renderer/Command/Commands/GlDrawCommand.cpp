/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlDrawCommand.hpp"

#include "Core/GlContextLock.hpp"

namespace ashes::gl4
{
	void apply( ContextLock const & context
		, CmdDraw const & cmd )
	{
		glLogCall( context
			, glDrawArraysInstancedBaseInstance
			, cmd.mode
			, cmd.firstVertex
			, cmd.vtxCount
			, cmd.instCount
			, cmd.firstInstance );
	}

	void buildDrawCommand( uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance
		, VkPrimitiveTopology mode
		, CmdList & list )
	{
		assert( instCount >= 1 );
		glLogCommand( "DrawCommand" );
		list.push_back( makeCmd< OpType::eDraw >( vtxCount
			, instCount
			, firstVertex
			, firstInstance
			, convert( mode ) ) );
	}
}
