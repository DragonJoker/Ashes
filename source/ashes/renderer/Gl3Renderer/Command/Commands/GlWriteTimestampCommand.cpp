/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlWriteTimestampCommand.hpp"

#include "Miscellaneous/GlQueryPool.hpp"

namespace gl_renderer
{
	WriteTimestampCommand::WriteTimestampCommand( Device const & device
		, VkPipelineStageFlagBits pipelineStage
		, ashes::QueryPool const & pool
		, uint32_t query )
		: CommandBase{ device }
		, m_query{ *( static_cast< QueryPool const & >( pool ).begin() + query ) }
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
