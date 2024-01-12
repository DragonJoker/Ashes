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
		, [[maybe_unused]] VkPipelineStageFlags srcStageMask
		, [[maybe_unused]] VkPipelineStageFlags dstStageMask
		, [[maybe_unused]] ArrayView< VkMemoryBarrier const > memoryBarriers
		, [[maybe_unused]] ArrayView< VkBufferMemoryBarrier const > bufferMemoryBarriers
		, [[maybe_unused]] ArrayView< VkImageMemoryBarrier const > imageMemoryBarriers
		, CmdList & list )
	{
		glLogCommand( list, "WaitEventsCommand" );
		list.push_back( makeCmd< OpType::eWaitEvents >( std::move( events ) ) );
	}
}
