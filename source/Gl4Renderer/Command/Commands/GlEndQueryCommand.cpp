/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlEndQueryCommand.hpp"

#include "Miscellaneous/GlQueryPool.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	EndQueryCommand::EndQueryCommand( VkDevice device
		, VkQueryPool pool
		, uint32_t query )
		: CommandBase{ device }
		, m_target{ convert( get( pool )->getType() ) }
	{
	}

	void EndQueryCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "EndQueryCommand" );
		glLogCall( context
			, glEndQuery
			, m_target );
	}

	CommandPtr EndQueryCommand::clone()const
	{
		return std::make_unique< EndQueryCommand >( *this );
	}
}
