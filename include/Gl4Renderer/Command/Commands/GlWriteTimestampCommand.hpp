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
	struct CmdConfig< OpType::eWriteTimestamp >
	{
		static Op constexpr value = { OpType::eWriteTimestamp, 2u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eWriteTimestamp >
	{
		inline CmdT( uint32_t name )
			: cmd{ { OpType::eWriteTimestamp, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
		{
		}

		Command cmd;
		uint32_t name;
	};
	using CmdWriteTimestamp = CmdT< OpType::eWriteTimestamp >;

	void apply( ContextLock const & context
		, CmdWriteTimestamp const & cmd );

	//*************************************************************************

	void buildWriteTimestampCommand( VkPipelineStageFlagBits pipelineStage
		, VkQueryPool pool
		, uint32_t query
		, CmdList & list );

	//*************************************************************************
}
