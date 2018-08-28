/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlResetEventCommand.hpp"

#include "Sync/GlEvent.hpp"

namespace gl_renderer
{
	ResetEventCommand::ResetEventCommand( Device const & device
		, renderer::Event const & event
		, renderer::PipelineStageFlags stageFlags )
		: CommandBase{ device }
		, m_event{ event }
	{
	}

	void ResetEventCommand::apply()const
	{
		glLogCommand( "ResetEventCommand" );
		m_event.reset();
	}

	CommandPtr ResetEventCommand::clone()const
	{
		return std::make_unique< ResetEventCommand >( *this );
	}
}
