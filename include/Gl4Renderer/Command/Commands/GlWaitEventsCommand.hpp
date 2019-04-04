/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
{
	class WaitEventsCommand
		: public CommandBase
	{
	public:
		WaitEventsCommand( VkDevice device
			, VkEventArray const & events
			, VkPipelineStageFlags srcStageMask
			, VkPipelineStageFlags dstStageMask
			, VkMemoryBarrierArray memoryBarriers
			, VkBufferMemoryBarrierArray bufferMemoryBarriers
			, VkImageMemoryBarrierArray imageMemoryBarriers );
		void apply( ContextLock const & context )const override;
		CommandPtr clone()const override;

	private:
		VkEventArray const & m_events;
	};
}
