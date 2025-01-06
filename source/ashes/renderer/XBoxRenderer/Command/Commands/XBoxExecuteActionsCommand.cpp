/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/XBoxExecuteActionsCommand.hpp"

#include "ashesxbox_api.hpp"

namespace ashes::xbox
{
	ExecuteActionsCommand::ExecuteActionsCommand( VkDevice device
		, ActionArray actions )
		: CommandBase{ device }
		, m_actions{ std::move( actions ) }
	{
	}

	void ExecuteActionsCommand::apply( Context const & context )const
	{
		for ( auto const & action : m_actions )
		{
			action( context );
		}
	}

	CommandPtr ExecuteActionsCommand::clone()const
	{
		return std::make_unique< ExecuteActionsCommand >( *this );
	}
}
