/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlWriteTimestampCommand.hpp"

#include "Miscellaneous/GlQueryPool.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	void apply( ContextLock const & context
		, CmdWriteTimestamp const & cmd )
	{
		glLogCall( context
			, glQueryCounter
			, cmd.name
			, GL_QUERY_TYPE_TIMESTAMP );
	}

	void buildWriteTimestampCommand( VkPipelineStageFlagBits pipelineStage
		, VkQueryPool pool
		, uint32_t query
		, CmdList & list )
	{
		glLogCommand( "WriteTimestampCommand" );
		list.push_back( makeCmd< OpType::eWriteTimestamp >( *( get( pool )->begin() + query ) ) );
	}
}
