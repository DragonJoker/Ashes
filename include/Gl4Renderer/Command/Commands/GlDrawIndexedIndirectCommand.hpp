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
	struct CmdConfig< OpType::eDrawIndexedIndirect >
	{
		static Op constexpr value = { OpType::eDrawIndexedIndirect, 7u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDrawIndexedIndirect >
	{
		inline CmdT( uint64_t offset
			, uint32_t drawCount
			, uint32_t stride
			, GlPrimitiveTopology mode
			, GlIndexType type )
			: cmd{ { OpType::eDrawIndexedIndirect, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, offset{ std::move( offset ) }
			, drawCount{ std::move( drawCount ) }
			, stride{ std::move( stride ) }
			, mode{ std::move( mode ) }
			, type{ std::move( type ) }
		{
		}

		Command cmd;
		uint64_t offset;
		uint32_t drawCount;
		uint32_t stride;
		GlPrimitiveTopology mode;
		GlIndexType type;
	};
	using CmdDrawIndexedIndirect = CmdT< OpType::eDrawIndexedIndirect >;

	void apply( ContextLock const & context
		, CmdDrawIndexedIndirect const & cmd );

	//*************************************************************************

	void buildDrawIndexedIndirectCommand( VkBuffer buffer
		, VkDeviceSize offset
		, uint32_t drawCount
		, uint32_t stride
		, VkPrimitiveTopology mode
		, VkIndexType type
		, CmdList & list );

	//*************************************************************************
}
