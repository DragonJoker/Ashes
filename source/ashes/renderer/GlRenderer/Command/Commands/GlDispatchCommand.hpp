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
	struct CmdConfig< OpType::eDispatch >
	{
		static Op constexpr value = { OpType::eDispatch, 4u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDispatch >
	{
		inline CmdT( uint32_t groupCountX
			, uint32_t groupCountY
			, uint32_t groupCountZ )
			: cmd{ { OpType::eDispatch, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, groupCountX{ std::move( groupCountX ) }
			, groupCountY{ std::move( groupCountY ) }
			, groupCountZ{ std::move( groupCountZ ) }
		{
		}

		Command cmd;
		uint32_t groupCountX;
		uint32_t groupCountY;
		uint32_t groupCountZ;
	};
	using CmdDispatch = CmdT< OpType::eDispatch >;

	void apply( ContextLock const & context
		, CmdDispatch const & cmd );

	//*************************************************************************

	void buildDispatchCommand( uint32_t groupCountX
		, uint32_t groupCountY
		, uint32_t groupCountZ
		, CmdList & list );

	//*************************************************************************
}
