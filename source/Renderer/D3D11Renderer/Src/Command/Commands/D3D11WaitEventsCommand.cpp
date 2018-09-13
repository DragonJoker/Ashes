/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11WaitEventsCommand.hpp"

#include "Sync/D3D11Event.hpp"

#include <algorithm>
#include <chrono>
#include <thread>

namespace d3d11_renderer
{
	WaitEventsCommand::WaitEventsCommand( Device const & device
		, renderer::EventCRefArray const & events
		, renderer::PipelineStageFlags srcStageMask
		, renderer::PipelineStageFlags dstStageMask
		, renderer::BufferMemoryBarrierArray const & bufferMemoryBarriers
		, renderer::ImageMemoryBarrierArray const & imageMemoryBarriers )
		: CommandBase{ device }
		, m_events{ events }
	{
	}

	void WaitEventsCommand::apply( Context const & context )const
	{
		auto count = 0u;

		do
		{
			count = uint32_t( std::count_if( m_events.begin()
				, m_events.end()
				, []( renderer::EventCRef const & event )
				{
					return event.get().getStatus() != renderer::EventStatus::eSet
						&& event.get().getStatus() != renderer::EventStatus::eError;
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
