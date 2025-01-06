/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/XBoxSetEventCommand.hpp"

#include "Sync/XBoxEvent.hpp"

#include "ashesxbox_api.hpp"

namespace ashes::xbox
{
	SetEventCommand::SetEventCommand( VkDevice device
		, VkEvent event )
		: CommandBase{ device }
		, m_event{ event }
	{
	}

	void SetEventCommand::apply( Context const & context )const
	{
		get( m_event )->reset();
	}

	CommandPtr SetEventCommand::clone()const
	{
		return std::make_unique< SetEventCommand >( *this );
	}
}
