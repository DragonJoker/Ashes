/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/XBoxResetEventCommand.hpp"

#include "Sync/XBoxEvent.hpp"

namespace ashes::xbox
{
	ResetEventCommand::ResetEventCommand( VkDevice device
		, VkEvent event )
		: CommandBase{ device }
		, m_event{ event }
	{
	}

	void ResetEventCommand::apply( Context const & context )const
	{
	}

	CommandPtr ResetEventCommand::clone()const
	{
		return std::make_unique< ResetEventCommand >( *this );
	}
}
