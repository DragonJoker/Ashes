/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/XBoxWriteTimestampCommand.hpp"

#include "Miscellaneous/XBoxQueryPool.hpp"

#include "ashesxbox_api.hpp"

namespace ashes::xbox
{
	WriteTimestampCommand::WriteTimestampCommand( VkDevice device
		, VkQueryPool pool
		, uint32_t query )
		: CommandBase{ device }
		, m_query{ *( get( pool )->begin() + query ) }
	{
	}

	void WriteTimestampCommand::apply( Context const & context )const
	{
		context.context->End( m_query );
	}

	CommandPtr WriteTimestampCommand::clone()const
	{
		return std::make_unique< WriteTimestampCommand >( *this );
	}
}
