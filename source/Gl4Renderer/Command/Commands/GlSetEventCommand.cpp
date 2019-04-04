/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlSetEventCommand.hpp"

#include "Sync/GlEvent.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	SetEventCommand::SetEventCommand( VkDevice device
		, VkEvent event
		, VkPipelineStageFlags stageFlags )
		: CommandBase{ device }
		, m_event{ event }
	{
	}

	void SetEventCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "SetEventCommand" );
		get( m_event )->reset();
	}

	CommandPtr SetEventCommand::clone()const
	{
		return std::make_unique< SetEventCommand >( *this );
	}
}
