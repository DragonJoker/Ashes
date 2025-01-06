/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/XBoxEndQueryCommand.hpp"

#include "Miscellaneous/XBoxQueryPool.hpp"

#include "ashesxbox_api.hpp"

namespace ashes::xbox
{
	EndQueryCommand::EndQueryCommand( VkDevice device
		, VkQueryPool pool
		, uint32_t query )
		: CommandBase{ device }
		, m_query{ *( get( pool )->begin() + query ) }
	{
	}

	void EndQueryCommand::apply( Context const & context )const
	{
		context.context->End( m_query );
	}

	CommandPtr EndQueryCommand::clone()const
	{
		return std::make_unique< EndQueryCommand >( *this );
	}
}
