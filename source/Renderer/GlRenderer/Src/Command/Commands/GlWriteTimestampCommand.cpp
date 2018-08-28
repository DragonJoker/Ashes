/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlWriteTimestampCommand.hpp"

#include "Miscellaneous/GlQueryPool.hpp"

namespace gl_renderer
{
	WriteTimestampCommand::WriteTimestampCommand( Device const & device
		, renderer::PipelineStageFlag pipelineStage
		, renderer::QueryPool const & pool
		, uint32_t query )
		: CommandBase{ device }
		, m_query{ *( static_cast< QueryPool const & >( pool ).begin() + query ) }
	{
	}

	void WriteTimestampCommand::apply()const
	{
		glLogCommand( "WriteTimestampCommand" );
		glLogCall( m_device.getContext(), glQueryCounter, m_query, GL_QUERY_TYPE_TIMESTAMP );
	}

	CommandPtr WriteTimestampCommand::clone()const
	{
		return std::make_unique< WriteTimestampCommand >( *this );
	}
}
