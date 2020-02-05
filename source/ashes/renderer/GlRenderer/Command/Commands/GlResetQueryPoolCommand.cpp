/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlResetQueryPoolCommand.hpp"

namespace ashes::gl
{
	void buildResetQueryPoolCommand( VkQueryPool pool
		, uint32_t firstQuery
		, uint32_t queryCount
		, CmdList & list )
	{
		glLogCommand( list, "ResetQueryPoolCommand" );
	}
}
