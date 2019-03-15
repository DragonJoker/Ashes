/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11ResetQueryPoolCommand.hpp"

namespace d3d11_renderer
{
	ResetQueryPoolCommand::ResetQueryPoolCommand( Device const & device
		, ashes::QueryPool const & pool
		, uint32_t firstQuery
		, uint32_t queryCount )
		: CommandBase{ device }
	{
	}

	void ResetQueryPoolCommand::apply( Context const & context )const
	{
	}

	CommandPtr ResetQueryPoolCommand::clone()const
	{
		return std::make_unique< ResetQueryPoolCommand >( *this );
	}
}
