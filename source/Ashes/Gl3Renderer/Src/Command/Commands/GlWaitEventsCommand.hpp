/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "GlCommandBase.hpp"

namespace gl_renderer
{
	class WaitEventsCommand
		: public CommandBase
	{
	public:
		WaitEventsCommand( Device const & device
			, ashes::EventCRefArray const & events
			, ashes::PipelineStageFlags srcStageMask
			, ashes::PipelineStageFlags dstStageMask
			, ashes::BufferMemoryBarrierArray const & bufferMemoryBarriers
			, ashes::ImageMemoryBarrierArray const & imageMemoryBarriers );
		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		ashes::EventCRefArray const & m_events;
	};
}
