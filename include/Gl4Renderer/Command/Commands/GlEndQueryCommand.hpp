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
	struct CmdConfig< OpType::eEndQuery >
	{
		static Op constexpr value = { OpType::eEndQuery, 2u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eEndQuery >
	{
		inline CmdT( uint32_t target )
			: cmd{ { OpType::eEndQuery, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
		{
		}

		Command cmd;
		uint32_t target;
	};
	using CmdEndQuery = CmdT< OpType::eEndQuery >;

	void apply( ContextLock const & context
		, CmdEndQuery const & cmd );

	//*************************************************************************

	void buildEndQueryCommand( VkQueryPool pool
		, uint32_t query
		, CmdList & list );

	//*************************************************************************
}
