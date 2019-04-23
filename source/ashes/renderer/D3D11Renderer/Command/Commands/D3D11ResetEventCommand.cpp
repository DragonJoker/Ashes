/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11ResetEventCommand.hpp"

#include "Sync/D3D11Event.hpp"

namespace ashes::d3d11
{
	ResetEventCommand::ResetEventCommand( Device const & device
		, ashes::Event const & event
		, VkPipelineStageFlags stageFlags )
		: CommandBase{ device }
		, m_event{ event }
	{
	}

	void ResetEventCommand::apply( Context const & context )const
	{
	}

	CommandPtr ResetEventCommand::clone()const
	{
		return std::make_unique< ResetEventCommand >( *this );
	}
}
