/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlEndQueryCommand.hpp"

#include "Miscellaneous/GlQueryPool.hpp"

namespace gl_renderer
{
	EndQueryCommand::EndQueryCommand( Device const & device
		, ashes::QueryPool const & pool
		, uint32_t query )
		: CommandBase{ device }
		, m_target{ convert( pool.getType() ) }
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
