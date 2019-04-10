/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
{
	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eResetEvent >
	{
		static Op constexpr value = { OpType::eResetEvent, 3u };
	};

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
