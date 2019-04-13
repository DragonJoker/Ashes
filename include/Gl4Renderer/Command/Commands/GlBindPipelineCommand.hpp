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
	struct CmdConfig< OpType::eBindContextState >
	{
		static Op constexpr value = { OpType::eBindContextState, 6u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindContextState >
	{
		inline CmdT( VkDevice device
			, ContextState const * state )
			: cmd{ { OpType::eBindContextState, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, device{ device }
			, state{ state }
		{
		}

		Command cmd;
		uint32_t dummy{ 0u }; // Here for alignment purpose
		VkDevice device;
		ContextState const * state;
	};
	using CmdBindContextState = CmdT< OpType::eBindContextState >;

	void apply( ContextLock const & context
		, CmdBindContextState const & cmd );

	//*************************************************************************

	void buildBindPipelineCommand( VkDevice device
		, VkPipeline pipeline
		, VkPipelineBindPoint bindingPoint
		, bool isRtot
		, CmdList & list );

	void buildUnbindPipelineCommand( VkDevice device
		, VkPipeline pipeline
		, VkImageView view
		, CmdList & list );

	//*************************************************************************
}
