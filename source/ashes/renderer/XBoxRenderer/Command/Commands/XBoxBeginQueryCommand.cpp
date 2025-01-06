/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/XBoxBeginQueryCommand.hpp"

#include "Miscellaneous/XBoxQueryPool.hpp"

#include "ashesxbox_api.hpp"

namespace ashes::xbox
{
	BeginQueryCommand::BeginQueryCommand( VkDevice device
		, VkQueryPool pool
		, uint32_t query )
		: CommandBase{ device }
		, m_query{ *( get( pool )->begin() + query ) }
	{
	}

	void BeginQueryCommand::apply( Context const & context )const
	{
		context.context->Begin( m_query );
	}

	CommandPtr BeginQueryCommand::clone()const
	{
		return std::make_unique< BeginQueryCommand >( *this );
	}
}
