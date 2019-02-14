/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace d3d11_renderer
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
		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		ashes::EventCRefArray const & m_events;
	};
}
