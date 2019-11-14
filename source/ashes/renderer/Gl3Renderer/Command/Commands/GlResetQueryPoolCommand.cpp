/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlResetQueryPoolCommand.hpp"

namespace ashes::gl3
{
	void buildResetQueryPoolCommand( VkQueryPool pool
		, uint32_t firstQuery
		, uint32_t queryCount
		, CmdList & list )
	{
		glLogCommand( "ResetQueryPoolCommand" );
	}
}
