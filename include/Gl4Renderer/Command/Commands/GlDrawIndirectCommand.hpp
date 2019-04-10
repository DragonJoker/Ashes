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
	struct CmdConfig< OpType::eDrawIndirect >
	{
		static Op constexpr value = { OpType::eDrawIndirect, 6u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDrawIndirect >
	{
		inline CmdT( uint64_t offset
			, uint32_t drawCount
			, uint32_t stride
			, GlPrimitiveTopology mode )
			: cmd{ { OpType::eDrawIndirect, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, offset{ std::move( offset ) }
			, drawCount{ std::move( drawCount ) }
			, stride{ std::move( stride ) }
			, mode{ std::move( mode ) }
		{
		}

		Command cmd;
		uint64_t offset;
		uint32_t drawCount;
		uint32_t stride;
		GlPrimitiveTopology mode;
	};
	using CmdDrawIndirect = CmdT< OpType::eDrawIndirect >;

	void apply( ContextLock const & context
		, CmdDrawIndirect const & cmd );

	//*************************************************************************

	void buildDrawIndirectCommand( VkBuffer buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride
		, VkPrimitiveTopology mode
		, CmdList & list );

	//*************************************************************************
}
