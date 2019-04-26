/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11SetEventCommand.hpp"

#include "Sync/D3D11Event.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	SetEventCommand::SetEventCommand( VkDevice device
		, VkEvent event
		, VkPipelineStageFlags stageFlags )
		: CommandBase{ device }
		, m_event{ event }
	{
	}

	void SetEventCommand::apply( Context const & context )const
	{
		get( m_event )->reset();
	}

	CommandPtr SetEventCommand::clone()const
	{
		return std::make_unique< SetEventCommand >( *this );
	}
}
