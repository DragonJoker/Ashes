/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlMemoryBarrierCommand.hpp"

namespace ashes::gl4
{
	void apply( ContextLock const & context
		, CmdMemoryBarrier const & cmd )
	{
		glLogCall( context
			, glMemoryBarrier
			, cmd.flags );
	}

	void buildMemoryBarrierCommand( VkPipelineStageFlags after
		, VkPipelineStageFlags before
		, VkDependencyFlags dependencyFlags
		, VkMemoryBarrierArray memoryBarriers
		, VkBufferMemoryBarrierArray bufferMemoryBarriers
		, VkImageMemoryBarrierArray imageMemoryBarriers
		, CmdList & list )
	{
		glLogCommand( "MemoryBarrierCommand" );
		list.push_back( makeCmd< OpType::eMemoryBarrier >( getMemoryBarrierFlags( before ) ) );
	}
}
