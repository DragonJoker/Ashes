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
	struct alignas( uint64_t ) CmdT< OpType::eResetEvent >
	{
		inline CmdT( VkEvent event )
			: cmd{ { OpType::eResetEvent, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, event{ std::move( event ) }
		{
		}

		Command cmd;
		VkEvent event;
	};
	using CmdResetEvent = CmdT< OpType::eResetEvent >;

	void apply( ContextLock const & context
		, CmdResetEvent const & cmd );

	//*************************************************************************

	void buildResetEventCommand( VkEvent event
		, VkPipelineStageFlags stageFlags
		, CmdList & list );

	//*************************************************************************
}
