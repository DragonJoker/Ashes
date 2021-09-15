/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestDispatchCommand.hpp"

namespace ashes::test
{
	DispatchCommand::DispatchCommand( VkDevice device
		, uint32_t
		, uint32_t
		, uint32_t )
		: CommandBase{ device }
	{
	}

	void DispatchCommand::apply()const
	{
	}

	CommandPtr DispatchCommand::clone()const
	{
		return std::make_unique< DispatchCommand >( *this );
	}
}
