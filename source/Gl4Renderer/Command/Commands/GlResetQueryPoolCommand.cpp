/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlResetQueryPoolCommand.hpp"

namespace ashes::gl4
{
	ResetQueryPoolCommand::ResetQueryPoolCommand( VkDevice device
		, VkQueryPool pool
		, uint32_t firstQuery
		, uint32_t queryCount )
		: CommandBase{ device }
	{
	}

	void ResetQueryPoolCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "ResetQueryPoolCommand" );
	}

	CommandPtr ResetQueryPoolCommand::clone()const
	{
		return std::make_unique< ResetQueryPoolCommand >( *this );
	}
}
