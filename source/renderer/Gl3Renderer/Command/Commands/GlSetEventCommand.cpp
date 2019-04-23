/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlSetEventCommand.hpp"

#include "Sync/GlEvent.hpp"

namespace gl_renderer
{
	SetEventCommand::SetEventCommand( Device const & device
		, ashes::Event const & event
		, VkPipelineStageFlags stageFlags )
		: CommandBase{ device }
		, m_event{ event }
	{
	}

	void SetEventCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "SetEventCommand" );
		m_event.reset();
	}

	CommandPtr SetEventCommand::clone()const
	{
		return std::make_unique< SetEventCommand >( *this );
	}
}
