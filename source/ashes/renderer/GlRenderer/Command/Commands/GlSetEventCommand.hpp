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
	struct CmdConfig< OpType::eSetEvent >
	{
		static Op constexpr value = { OpType::eSetEvent, 3u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eSetEvent >
	{
		inline CmdT( VkEvent event )
			: cmd{ { OpType::eSetEvent, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, event{ std::move( event ) }
		{
		}

		Command cmd;
		VkEvent event;
	};
	using CmdSetEvent = CmdT< OpType::eSetEvent >;

	void apply( ContextLock const & context
		, CmdSetEvent const & cmd );

	//*************************************************************************

	void buildSetEventCommand( VkEvent event
		, VkPipelineStageFlags stageFlags
		, CmdList & list );

	//*************************************************************************
}
