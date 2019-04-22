/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11SetEventCommand.hpp"

#include "Sync/D3D11Event.hpp"

namespace ashes::d3d11
{
	SetEventCommand::SetEventCommand( Device const & device
		, ashes::Event const & event
		, ashes::PipelineStageFlags stageFlags )
		: CommandBase{ device }
		, m_event{ event }
	{
	}

	void SetEventCommand::apply( Context const & context )const
	{
		m_event.reset();
	}

	CommandPtr SetEventCommand::clone()const
	{
		return std::make_unique< SetEventCommand >( *this );
	}
}
