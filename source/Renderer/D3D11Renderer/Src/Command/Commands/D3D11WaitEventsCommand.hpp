/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11CommandBase.hpp"

namespace d3d11_renderer
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
		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		renderer::EventCRefArray const & m_events;
	};
}
