/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlSetEventCommand.hpp"

#include "Sync/GlEvent.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	void buildSetEventCommand( VkEvent event
		, [[maybe_unused]] VkPipelineStageFlags stageFlags
		, CmdList & list )
	{
		glLogCommand( list, "SetEventCommand" );
		list.push_back( makeCmd< OpType::eSetEvent >( event ) );
	}
}
