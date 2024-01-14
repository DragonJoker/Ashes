/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestResetQueryPoolCommand.hpp"

namespace ashes::test
{
	ResetQueryPoolCommand::ResetQueryPoolCommand( VkDevice device
		, VkQueryPool
		, uint32_t
		, uint32_t )
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
