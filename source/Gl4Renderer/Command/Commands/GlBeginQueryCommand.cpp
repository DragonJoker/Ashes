/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBeginQueryCommand.hpp"

#include "Miscellaneous/GlQueryPool.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	BeginQueryCommand::BeginQueryCommand( VkDevice device
		, VkQueryPool pool
		, uint32_t query
		, VkQueryControlFlags flags )
		: CommandBase{ device }
		, m_target{ convert( get( pool )->getType() ) }
		, m_query{ *( get( pool )->begin() + query ) }
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
