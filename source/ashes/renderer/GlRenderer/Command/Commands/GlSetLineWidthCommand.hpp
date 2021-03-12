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
	struct alignas( uint64_t ) CmdT< OpType::eSetLineWidth >
	{
		inline CmdT( float width )
			: cmd{ { OpType::eSetLineWidth, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, width{ std::move( width ) }
		{
		}

		Command cmd;
		float width;
	};
	using CmdSetLineWidth = CmdT< OpType::eSetLineWidth >;

	void apply( ContextLock const & context
		, CmdSetLineWidth const & cmd );

	//*************************************************************************

	void buildSetLineWidthCommand( float width
		, CmdList & list );

	//*************************************************************************
}
