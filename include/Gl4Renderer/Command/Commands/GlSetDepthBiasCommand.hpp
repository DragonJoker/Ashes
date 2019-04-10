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
	struct CmdConfig< OpType::eSetDepthBias >
	{
		static Op constexpr value = { OpType::eSetDepthBias, 4u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eSetDepthBias >
	{
		inline CmdT( float constantFactor
			, float clamp
			, float slopeFactor )
			: cmd{ { OpType::eSetDepthBias, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, constantFactor{ std::move( constantFactor ) }
			, clamp{ std::move( clamp ) }
			, slopeFactor{ std::move( slopeFactor ) }
		{
		}

		Command cmd;
		float constantFactor;
		float clamp;
		float slopeFactor;
	};
	using CmdSetDepthBias = CmdT< OpType::eSetDepthBias >;

	void apply( ContextLock const & context
		, CmdSetDepthBias const & cmd );

	//*************************************************************************

	void buildSetDepthBiasCommand( float constantFactor
		, float clamp
		, float slopeFactor
		, CmdList & list );

	//*************************************************************************
}
