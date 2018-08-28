/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlSetEventCommand.hpp"

#include "Sync/GlEvent.hpp"

namespace gl_renderer
{
	SetEventCommand::SetEventCommand( Device const & device
		, renderer::Event const & event
		, renderer::PipelineStageFlags stageFlags )
		: CommandBase{ device }
		, m_event{ event }
	{
	}

	void SetEventCommand::apply()const
	{
		glLogCommand( "SetEventCommand" );
		m_event.reset();
	}

	CommandPtr SetEventCommand::clone()const
	{
		return std::make_unique< SetEventCommand >( *this );
	}
}
