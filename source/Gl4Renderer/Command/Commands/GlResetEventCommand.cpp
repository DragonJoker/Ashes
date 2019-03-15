/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlResetEventCommand.hpp"

#include "Sync/GlEvent.hpp"

namespace gl_renderer
{
	ResetEventCommand::ResetEventCommand( Device const & device
		, ashes::Event const & event
		, ashes::PipelineStageFlags stageFlags )
		: CommandBase{ device }
		, m_event{ event }
	{
	}

	void ResetEventCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "ResetEventCommand" );
		m_event.reset();
	}

	CommandPtr ResetEventCommand::clone()const
	{
		return std::make_unique< ResetEventCommand >( *this );
	}
}
