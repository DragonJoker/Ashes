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
		if ( get( pool )->getType() == VK_QUERY_TYPE_PIPELINE_STATISTICS )
		{
			assert( query == 0u );

			for ( auto type : get( pool )->getTypes() )
			{
				list.push_back( makeCmd< OpType::eEndQuery >( type ) );
				++query;
			}
		}
		else
		{
			list.push_back( makeCmd< OpType::eEndQuery >( convert( get( pool )->getType() ) ) );
		}
	}
}
