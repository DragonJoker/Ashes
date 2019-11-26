/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestEndQueryCommand.hpp"

namespace ashes::test
{
	EndQueryCommand::EndQueryCommand( VkDevice device
		, VkQueryPool pool
		, uint32_t query )
		: CommandBase{ device }
	{
	}

	void EndQueryCommand::apply()const
	{
	}

	CommandPtr EndQueryCommand::clone()const
	{
		return std::make_unique< EndQueryCommand >( *this );
	}
}
