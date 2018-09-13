/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlBufferMemoryBarrierCommand.hpp"

namespace gl_renderer
{
	BufferMemoryBarrierCommand::BufferMemoryBarrierCommand( Device const & device
		, ashes::PipelineStageFlags after
		, ashes::PipelineStageFlags before
		, ashes::BufferMemoryBarrier const & transitionBarrier )
		: CommandBase{ device }
		, m_flags{ convert( before ) }
	{
	}

	void BufferMemoryBarrierCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "BufferMemoryBarrierCommand" );
		glLogCall( context
			, glMemoryBarrier
			, m_flags );
	}

	CommandPtr BufferMemoryBarrierCommand::clone()const
	{
		return std::make_unique< BufferMemoryBarrierCommand >( *this );
	}
}
