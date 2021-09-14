/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlDrawCommand.hpp"

#include "Core/GlContextLock.hpp"

namespace ashes::gl
{
	void buildDrawCommand( uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance
		, VkPrimitiveTopology mode
		, CmdList & list )
	{
		if ( vtxCount == 0 || instCount == 0 )
		{
			return;
		}

		glLogCommand( list, "DrawCommand" );

		if ( firstInstance > 0 )
		{
			list.push_back( makeCmd< OpType::eDrawBaseInstance >( vtxCount
				, instCount
				, firstVertex
				, firstInstance
				, convert( mode ) ) );
		}
		else
		{
			list.push_back( makeCmd< OpType::eDraw >( vtxCount
				, instCount
				, firstVertex
				, convert( mode ) ) );
		}
	}
}
