/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class WaitEventsCommand
		: public CommandBase
	{
	public:
		WaitEventsCommand( VkDevice device
			, VkEventArray const & events
			, VkPipelineStageFlags srcStageMask
			, VkPipelineStageFlags dstStageMask
			, VkBufferMemoryBarrierArray const & bufferMemoryBarriers
			, VkImageMemoryBarrierArray const & imageMemoryBarriers );
		void apply()const;
		CommandPtr clone()const;

	private:
		VkEventArray m_events;
	};
}
