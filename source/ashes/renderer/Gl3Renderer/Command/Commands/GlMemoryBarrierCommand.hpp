/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl3Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl3
{
	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eMemoryBarrier >
	{
		static Op constexpr value = { OpType::eMemoryBarrier, 2u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eMemoryBarrier >
	{
		inline CmdT( uint32_t flags )
			: cmd{ { OpType::eMemoryBarrier, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, flags{ std::move( flags ) }
		{
		}

		Command cmd;
		uint32_t flags;
	};
	using CmdMemoryBarrier = CmdT< OpType::eMemoryBarrier >;

	void apply( ContextLock const & context
		, CmdMemoryBarrier const & cmd );
	
	//*************************************************************************

	void buildMemoryBarrierCommand( VkPipelineStageFlags after
		, VkPipelineStageFlags before
		, VkDependencyFlags dependencyFlags
		, VkMemoryBarrierArray memoryBarriers
		, VkBufferMemoryBarrierArray bufferMemoryBarriers
		, VkImageMemoryBarrierArray imageMemoryBarriers
		, CmdList & list );

	//*************************************************************************
}
