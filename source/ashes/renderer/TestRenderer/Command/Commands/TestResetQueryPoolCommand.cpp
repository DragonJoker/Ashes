/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestResetQueryPoolCommand.hpp"

namespace ashes::test
{
	ResetQueryPoolCommand::ResetQueryPoolCommand( VkDevice device
		, VkQueryPool pool
		, uint32_t firstQuery
		, uint32_t queryCount )
		: CommandBase{ device }
	{
	}

	void ResetQueryPoolCommand::apply()const
	{
	}

	CommandPtr ResetQueryPoolCommand::clone()const
	{
		return std::make_unique< ResetQueryPoolCommand >( *this );
	}
}
