/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlResetEventCommand.hpp"

#include "Sync/GlEvent.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	void buildResetEventCommand( VkEvent event
		, [[maybe_unused]] VkPipelineStageFlags stageFlags
		, CmdList & list )
	{
		glLogCommand( list, "ResetEventCommand" );
		list.push_back( makeCmd< OpType::eResetEvent >( event ) );
	}
}
