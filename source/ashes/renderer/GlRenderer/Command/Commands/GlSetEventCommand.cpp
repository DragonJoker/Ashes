/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlSetEventCommand.hpp"

#include "Sync/GlEvent.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
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
		glLogCommand( list, "SetEventCommand" );
		list.push_back( makeCmd< OpType::eSetEvent >( event ) );
	}
}
