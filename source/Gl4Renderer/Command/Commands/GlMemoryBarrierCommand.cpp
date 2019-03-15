/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlMemoryBarrierCommand.hpp"

namespace gl_renderer
{
	MemoryBarrierCommand::MemoryBarrierCommand( Device const & device
		, ashes::PipelineStageFlags after
		, ashes::PipelineStageFlags before
		, ashes::DependencyFlags dependencyFlags
		, ashes::MemoryBarrierArray const & memoryBarriers
		, ashes::BufferMemoryBarrierArray const & bufferMemoryBarriers
		, ashes::ImageMemoryBarrierArray const & imageMemoryBarriers )
		: CommandBase{ device }
		, m_flags{ convert( before ) }
	{
	}

	void MemoryBarrierCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "MemoryBarrierCommand" );
		glLogCall( context
			, glMemoryBarrier
			, m_flags );
	}

	CommandPtr MemoryBarrierCommand::clone()const
	{
		return std::make_unique< MemoryBarrierCommand >( *this );
	}
}
