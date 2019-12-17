/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
{
	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eBindContextState >
	{
		static Op constexpr value = { OpType::eBindContextState, 6u };
	};

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
		, CmdList & list );

	void buildUnbindPipelineCommand( ContextStateStack & stack
		, CmdList & list );

	//*************************************************************************
}
