/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlMemoryBarrierCommand.hpp"

namespace ashes::gl4
{
	MemoryBarrierCommand::MemoryBarrierCommand( VkDevice device
		, VkPipelineStageFlags after
		, VkPipelineStageFlags before
		, VkDependencyFlags dependencyFlags
		, VkMemoryBarrierArray memoryBarriers
		, VkBufferMemoryBarrierArray bufferMemoryBarriers
		, VkImageMemoryBarrierArray imageMemoryBarriers )
		: CommandBase{ device }
		, m_flags{ getMemoryBarrierFlags( before ) }
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
