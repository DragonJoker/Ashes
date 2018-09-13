/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlWaitEventsCommand.hpp"

#include "Sync/GlEvent.hpp"

#include <algorithm>
#include <chrono>
#include <thread>

namespace gl_renderer
{
	WaitEventsCommand::WaitEventsCommand( Device const & device
		, ashes::EventCRefArray const & events
		, ashes::PipelineStageFlags srcStageMask
		, ashes::PipelineStageFlags dstStageMask
		, ashes::BufferMemoryBarrierArray const & bufferMemoryBarriers
		, ashes::ImageMemoryBarrierArray const & imageMemoryBarriers )
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
				, []( ashes::EventCRef const & event )
				{
					return event.get().getStatus() != ashes::EventStatus::eSet
						&& event.get().getStatus() != ashes::EventStatus::eError;
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
