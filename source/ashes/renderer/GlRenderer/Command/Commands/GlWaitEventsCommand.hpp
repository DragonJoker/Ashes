/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl
{
	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eWaitEvents >
	{
		static Op constexpr value = { OpType::eWaitEvents, 1u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eWaitEvents >
	{
		inline CmdT( VkEventArray events )
			: cmd{ { OpType::eWaitEvents, uint16_t( sizeof( Command ) + events.size() * 2u ) } }
			, events{ std::move( events ) }
		{
		}

		Command cmd;
		VkEventArray events;
	};
	using CmdWaitEvents = CmdT< OpType::eWaitEvents >;

	void apply( ContextLock const & context
		, CmdWaitEvents const & cmd );

	//*************************************************************************

	void buildWaitEventsCommand( VkEventArray events
		, VkPipelineStageFlags srcStageMask
		, VkPipelineStageFlags dstStageMask
		, VkMemoryBarrierArray memoryBarriers
		, VkBufferMemoryBarrierArray bufferMemoryBarriers
		, VkImageMemoryBarrierArray imageMemoryBarriers
		, CmdList & list );

	//*************************************************************************
}
