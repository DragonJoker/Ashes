/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11ResetEventCommand.hpp"

#include "Sync/D3D11Event.hpp"

namespace ashes::d3d11
{
	ResetEventCommand::ResetEventCommand( VkDevice device
		, VkEvent event )
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
