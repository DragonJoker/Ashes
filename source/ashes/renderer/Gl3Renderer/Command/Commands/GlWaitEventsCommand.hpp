/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl3Renderer/Command/Commands/GlCommandBase.hpp"

namespace gl_renderer
{
	class WaitEventsCommand
		: public CommandBase
	{
	public:
		WaitEventsCommand( Device const & device
			, ashes::EventCRefArray const & events
			, VkPipelineStageFlags srcStageMask
			, VkPipelineStageFlags dstStageMask
			, ashes::BufferMemoryBarrierArray const & bufferMemoryBarriers
			, ashes::VkImageMemoryBarrierArray const & imageMemoryBarriers );
		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		ashes::EventCRefArray const & m_events;
	};
}
