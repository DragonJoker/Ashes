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
	struct CmdConfig< OpType::eBeginQuery >
	{
		static Op constexpr value = { OpType::eBeginQuery, 3u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBeginQuery >
	{
		inline CmdT( uint32_t target
			, uint32_t query )
			: cmd{ { OpType::eBeginQuery, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
			, query{ std::move( query ) }
		{
		}

		Command cmd;
		uint32_t target;
		uint32_t query;
	};
	using CmdBeginQuery = CmdT< OpType::eBeginQuery >;

	void apply( ContextLock const & context
		, CmdBeginQuery const & cmd );

	//*************************************************************************

	void buildBeginQueryCommand( VkQueryPool pool
		, uint32_t query
		, CmdList & list );

	//*************************************************************************
}
