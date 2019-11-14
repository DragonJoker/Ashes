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
	struct CmdConfig< OpType::eDraw >
	{
		static Op constexpr value = { OpType::eDraw, 6u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDraw >
	{
		inline CmdT( uint32_t vtxCount
			, uint32_t instCount
			, uint32_t firstVertex
			, uint32_t firstInstance
			, GlPrimitiveTopology mode )
			: cmd{ { OpType::eDraw, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, vtxCount{ std::move( vtxCount ) }
			, instCount{ std::move( instCount ) }
			, firstVertex{ std::move( firstVertex ) }
			, firstInstance{ std::move( firstInstance ) }
			, mode{ std::move( mode ) }
		{
		}

		Command cmd;
		uint32_t vtxCount;
		uint32_t instCount;
		uint32_t firstVertex;
		uint32_t firstInstance;
		GlPrimitiveTopology mode;
	};
	using CmdDraw = CmdT< OpType::eDraw >;

	void apply( ContextLock const & context
		, CmdDraw const & cmd );

	//*************************************************************************

	void buildDrawCommand( uint32_t vtxCount
		, uint32_t instCount
		, uint32_t firstVertex
		, uint32_t firstInstance
		, VkPrimitiveTopology mode
		, CmdList & list );

	//*************************************************************************
}
