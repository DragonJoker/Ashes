/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlWaitEventsCommand.hpp"

#include "Sync/GlEvent.hpp"

#include <algorithm>
#include <chrono>
#include <thread>

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	void buildWaitEventsCommand( VkEventArray events
		, VkPipelineStageFlags srcStageMask
		, VkPipelineStageFlags dstStageMask
		, ArrayView< VkMemoryBarrier const > memoryBarriers
		, ArrayView< VkBufferMemoryBarrier const > bufferMemoryBarriers
		, ArrayView< VkImageMemoryBarrier const > imageMemoryBarriers
		, CmdList & list )
	{
		glLogCommand( list, "WaitEventsCommand" );
		list.push_back( makeCmd< OpType::eWaitEvents >( std::move( events ) ) );
	}
}
