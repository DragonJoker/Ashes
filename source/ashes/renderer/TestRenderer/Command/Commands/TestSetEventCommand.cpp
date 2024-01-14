/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestSetEventCommand.hpp"

#include "Sync/TestEvent.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	SetEventCommand::SetEventCommand( VkDevice device
		, VkEvent event
		, VkPipelineStageFlags )
		: CommandBase{ device }
		, m_event{ event }
	{
	}

	void SetEventCommand::apply()const
	{
		get( m_event )->reset();
	}

	CommandPtr SetEventCommand::clone()const
	{
		return std::make_unique< SetEventCommand >( *this );
	}
}
