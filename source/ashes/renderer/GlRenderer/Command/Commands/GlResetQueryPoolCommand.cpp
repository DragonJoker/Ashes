/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlResetQueryPoolCommand.hpp"

namespace ashes::gl
{
	void buildResetQueryPoolCommand( [[maybe_unused]] VkQueryPool pool
		, [[maybe_unused]] uint32_t firstQuery
		, [[maybe_unused]] uint32_t queryCount
		, [[maybe_unused]] CmdList & list )
	{
		glLogCommand( list, "ResetQueryPoolCommand" );
	}
}
