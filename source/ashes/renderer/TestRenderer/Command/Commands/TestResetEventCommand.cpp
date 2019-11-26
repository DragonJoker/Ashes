/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestResetEventCommand.hpp"

namespace ashes::test
{
	ResetEventCommand::ResetEventCommand( VkDevice device
		, VkEvent event
		, VkPipelineStageFlags stageFlags )
		: CommandBase{ device }
		, m_event{ event }
	{
	}

	void ResetEventCommand::apply()const
	{
	}

	CommandPtr ResetEventCommand::clone()const
	{
		return std::make_unique< ResetEventCommand >( *this );
	}
}
