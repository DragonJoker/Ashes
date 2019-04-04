/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlWriteTimestampCommand.hpp"

#include "Miscellaneous/GlQueryPool.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	WriteTimestampCommand::WriteTimestampCommand( VkDevice device
		, VkPipelineStageFlagBits pipelineStage
		, VkQueryPool pool
		, uint32_t query )
		: CommandBase{ device }
		, m_query{ *( get( pool )->begin() + query ) }
	{
	}

	void WriteTimestampCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "WriteTimestampCommand" );
		glLogCall( context
			, glQueryCounter
			, m_query
			, GL_QUERY_TYPE_TIMESTAMP );
	}

	CommandPtr WriteTimestampCommand::clone()const
	{
		return std::make_unique< WriteTimestampCommand >( *this );
	}
}
