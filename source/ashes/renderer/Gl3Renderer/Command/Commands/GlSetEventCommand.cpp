/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlSetEventCommand.hpp"

#include "Sync/GlEvent.hpp"

#include "ashesgl3_api.hpp"

namespace ashes::gl3
{
	void apply( ContextLock const & context
		, CmdSetEvent const & cmd )
	{
		get( cmd.event )->reset();
	}

	void buildSetEventCommand( VkEvent event
		, VkPipelineStageFlags stageFlags
		, CmdList & list )
	{
		glLogCommand( "SetEventCommand" );
		list.push_back( makeCmd< OpType::eSetEvent >( event ) );
	}
}
