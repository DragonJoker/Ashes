/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlEndQueryCommand.hpp"

#include "Miscellaneous/GlQueryPool.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	void apply( ContextLock const & context
		, CmdEndQuery const & cmd )
	{
		glLogCall( context
			, glEndQuery
			, cmd.target );
	}

	void buildEndQueryCommand( VkQueryPool pool
		, uint32_t query
		, CmdList & list )
	{
		glLogCommand( "EndQueryCommand" );
		list.push_back( makeCmd< OpType::eEndQuery >( convert( get( pool )->getType() ) ) );
	}
}
