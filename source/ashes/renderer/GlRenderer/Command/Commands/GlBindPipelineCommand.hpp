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
	struct alignas( uint64_t ) CmdT< OpType::eBindContextState >
	{
		inline CmdT( ContextStateStack * stack
			, ContextState * state )
			: cmd{ { OpType::eBindContextState, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, stack{ stack }
			, state{ state }
		{
		}

		Command cmd;
		ContextStateStack * stack;
		ContextState * state;
	};
	using CmdBindContextState = CmdT< OpType::eBindContextState >;

	void apply( ContextLock const & context
		, CmdBindContextState const & cmd );
	
	//*************************************************************************

	void buildBindPipelineCommand( ContextStateStack & stack
		, VkDevice device
		, VkPipeline pipeline
		, VkPipelineBindPoint bindingPoint
		, CmdList & list
		, PreExecuteActions & preExecuteActions );

	void buildUnbindPipelineCommand( ContextStateStack & stack
		, VkDevice device
		, CmdList & list );

	//*************************************************************************
}
