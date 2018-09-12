/*
This file belongs to RendererLib.
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
			, renderer::EventCRefArray const & events
			, renderer::PipelineStageFlags srcStageMask
			, renderer::PipelineStageFlags dstStageMask
			, renderer::BufferMemoryBarrierArray const & bufferMemoryBarriers
			, renderer::ImageMemoryBarrierArray const & imageMemoryBarriers );
		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		renderer::EventCRefArray const & m_events;
	};
}
