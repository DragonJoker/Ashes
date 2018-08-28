/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlResetQueryPoolCommand.hpp"

namespace gl_renderer
{
	ResetQueryPoolCommand::ResetQueryPoolCommand( Device const & device
		, renderer::QueryPool const & pool
		, uint32_t firstQuery
		, uint32_t queryCount )
		: CommandBase{ device }
	{
	}

	void ResetQueryPoolCommand::apply()const
	{
		glLogCommand( "ResetQueryPoolCommand" );
	}

	CommandPtr ResetQueryPoolCommand::clone()const
	{
		return std::make_unique< ResetQueryPoolCommand >( *this );
	}
}
