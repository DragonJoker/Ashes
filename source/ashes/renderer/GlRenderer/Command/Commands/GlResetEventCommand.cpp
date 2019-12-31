/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlResetEventCommand.hpp"

#include "Sync/GlEvent.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	void apply( ContextLock const & context
		, CmdResetEvent const & cmd )
	{
		get( cmd.event )->reset();
	}

	void buildResetEventCommand( VkEvent event
		, VkPipelineStageFlags stageFlags
		, CmdList & list )
	{
		glLogCommand( "ResetEventCommand" );
		list.push_back( makeCmd< OpType::eResetEvent >( event ) );
	}
}
