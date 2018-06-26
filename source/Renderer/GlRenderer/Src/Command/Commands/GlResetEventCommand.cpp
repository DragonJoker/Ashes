/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlResetEventCommand.hpp"

#include "Sync/GlEvent.hpp"

namespace gl_renderer
{
	ResetEventCommand::ResetEventCommand( renderer::Event const & event
		, renderer::PipelineStageFlags stageFlags )
		: m_event{ event }
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
