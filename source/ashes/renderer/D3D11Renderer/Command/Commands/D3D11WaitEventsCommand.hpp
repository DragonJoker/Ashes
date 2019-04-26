/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
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
		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		VkEventArray m_events;
	};
}
