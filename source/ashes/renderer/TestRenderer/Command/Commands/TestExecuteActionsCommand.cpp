/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestExecuteActionsCommand.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	ExecuteActionsCommand::ExecuteActionsCommand( VkDevice device
		, ActionArray actions )
		: CommandBase{ device }
		, m_actions{ std::move( actions ) }
	{
	}

	void ExecuteActionsCommand::apply()const
	{
		for ( auto & action : m_actions )
		{
			action();
		}
	}

	CommandPtr ExecuteActionsCommand::clone()const
	{
		return std::make_unique< ExecuteActionsCommand >( *this );
	}
}
