/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlWaitEventsCommand.hpp"

#include "Sync/GlEvent.hpp"

#include <algorithm>
#include <chrono>
#include <thread>

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	WaitEventsCommand::WaitEventsCommand( VkDevice device
		, VkEventArray const & events
		, VkPipelineStageFlags srcStageMask
		, VkPipelineStageFlags dstStageMask
		, VkMemoryBarrierArray memoryBarriers
		, VkBufferMemoryBarrierArray bufferMemoryBarriers
		, VkImageMemoryBarrierArray imageMemoryBarriers )
		: CommandBase{ device }
		, m_events{ events }
	{
	}

	void WaitEventsCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "WaitEventsCommand" );
		auto count = 0u;

		do
		{
			count = uint32_t( std::count_if( m_events.begin()
				, m_events.end()
				, []( VkEvent event )
				{
					return get( event )->getStatus() != VK_EVENT_SET
						&& get( event )->getStatus() != VK_EVENT_RESET;
				} ) );
			std::this_thread::sleep_for( std::chrono::nanoseconds{ 10 } );
		}
		while ( count != m_events.size() );
	}

	CommandPtr WaitEventsCommand::clone()const
	{
		return std::make_unique< WaitEventsCommand >( *this );
	}
}
