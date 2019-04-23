/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBeginQueryCommand.hpp"

#include "Miscellaneous/GlQueryPool.hpp"

namespace gl_renderer
{
	BeginQueryCommand::BeginQueryCommand( Device const & device
		, ashes::QueryPool const & pool
		, uint32_t query
		, ashes::QueryControlFlags flags )
		: CommandBase{ device }
		, m_target{ convert( pool.getType() ) }
		, m_query{ *( static_cast< QueryPool const & >( pool ).begin() + query ) }
	{
	}

	void BeginQueryCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "BeginQueryCommand" );
		glLogCall( context
			, glBeginQuery
			, m_target
			, m_query );
	}

	CommandPtr BeginQueryCommand::clone()const
	{
		return std::make_unique< BeginQueryCommand >( *this );
	}
}
