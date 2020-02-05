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
	struct CmdConfig< OpType::eDrawIndexed >
	{
		static Op constexpr value = { OpType::eDrawIndexed, 8u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDrawIndexed >
	{
		inline CmdT( uint32_t indexCount
			, uint32_t instCount
			, uint32_t indexOffset
			, uint32_t vertexOffset
			, GlPrimitiveTopology mode
			, GlIndexType type )
			: cmd{ { OpType::eDrawIndexed, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, indexCount{ std::move( indexCount ) }
			, instCount{ std::move( instCount ) }
			, indexOffset{ std::move( indexOffset ) }
			, vertexOffset{ std::move( vertexOffset ) }
			, mode{ std::move( mode ) }
			, type{ std::move( type ) }
		{
		}

		Command cmd;
		uint32_t indexCount;
		uint32_t instCount;
		uint32_t indexOffset;
		uint32_t vertexOffset;
		GlPrimitiveTopology mode;
		GlIndexType type;
	};
	using CmdDrawIndexed = CmdT< OpType::eDrawIndexed >;

	void apply( ContextLock const & context
		, CmdDrawIndexed const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eDrawIndexedBaseInstance >
	{
		static Op constexpr value = { OpType::eDrawIndexedBaseInstance, 8u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDrawIndexedBaseInstance >
	{
		inline CmdT( uint32_t indexCount
			, uint32_t instCount
			, uint32_t indexOffset
			, uint32_t vertexOffset
			, uint32_t firstInstance
			, GlPrimitiveTopology mode
			, GlIndexType type )
			: cmd{ { OpType::eDrawIndexedBaseInstance, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, indexCount{ std::move( indexCount ) }
			, instCount{ std::move( instCount ) }
			, indexOffset{ std::move( indexOffset ) }
			, vertexOffset{ std::move( vertexOffset ) }
			, firstInstance{ std::move( firstInstance ) }
			, mode{ std::move( mode ) }
			, type{ std::move( type ) }
		{
		}

		Command cmd;
		uint32_t indexCount;
		uint32_t instCount;
		uint32_t indexOffset;
		uint32_t vertexOffset;
		uint32_t firstInstance;
		GlPrimitiveTopology mode;
		GlIndexType type;
	};
	using CmdDrawIndexedBaseInstance = CmdT< OpType::eDrawIndexedBaseInstance >;

	void apply( ContextLock const & context
		, CmdDrawIndexedBaseInstance const & cmd );

	//*************************************************************************

	void buildDrawIndexedCommand( uint32_t indexCount
		, uint32_t instCount
		, uint32_t firstIndex
		, uint32_t vertexOffset
		, uint32_t firstInstance
		, VkPrimitiveTopology mode
		, VkIndexType type
		, CmdList & list );

	//*************************************************************************
}
