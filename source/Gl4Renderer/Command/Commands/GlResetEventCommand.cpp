/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlResetEventCommand.hpp"

#include "Sync/GlEvent.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	ResetEventCommand::ResetEventCommand( VkDevice device
		, VkEvent event
		, VkPipelineStageFlags stageFlags )
		: CommandBase{ device }
		, m_event{ event }
	{
	}

	void ResetEventCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "ResetEventCommand" );
		get( m_event )->reset();
	}

	CommandPtr ResetEventCommand::clone()const
	{
		return std::make_unique< ResetEventCommand >( *this );
	}
}
