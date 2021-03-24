/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBeginQueryCommand.hpp"

#include "Miscellaneous/GlQueryPool.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	void apply( ContextLock const & context
		, CmdBeginQuery const & cmd )
	{
		glLogCall( context
			, glBeginQuery
			, GlQueryType( cmd.target )
			, GLuint( cmd.query ) );
	}

	void buildBeginQueryCommand( VkQueryPool pool
		, uint32_t query
		, CmdList & list )
	{
		glLogCommand( list, "BeginQueryCommand" );

		if ( get( pool )->getType() == VK_QUERY_TYPE_PIPELINE_STATISTICS )
		{
			for ( auto type : get( pool )->getTypes() )
			{
				list.push_back( makeCmd< OpType::eBeginQuery >( type
					, *( get( pool )->begin() + query ) ) );
				++query;
			}
		}
		else
		{
			list.push_back( makeCmd< OpType::eBeginQuery >( convert( get( pool )->getType() )
				, *( get( pool )->begin() + query ) ) );
		}
	}
}
