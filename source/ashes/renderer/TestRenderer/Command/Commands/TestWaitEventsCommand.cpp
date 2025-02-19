/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestWaitEventsCommand.hpp"

#include "Sync/TestEvent.hpp"

#include <algorithm>
#include <chrono>
#include <thread>

#include "ashestest_api.hpp"

namespace ashes::test
{
	WaitEventsCommand::WaitEventsCommand( VkDevice device
		, VkEventArray const & events
		, VkPipelineStageFlags
		, VkPipelineStageFlags
		, VkBufferMemoryBarrierArray const &
		, VkImageMemoryBarrierArray const & )
		: CommandBase{ device }
		, m_events{ events }
	{
	}

	void WaitEventsCommand::apply()const
	{
		auto count = 0u;

		do
		{
			count = uint32_t( std::count_if( m_events.begin()
				, m_events.end()
				, []( VkEvent const & event )
				{
					return get( event )->getStatus() != VK_EVENT_SET
						&& get( event )->getStatus() != VK_TIMEOUT;
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
