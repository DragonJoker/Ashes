/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl3Renderer/Command/Commands/GlCommandBase.hpp"

#include "renderer/Gl3Renderer/Enum/GlFormat.hpp"

namespace ashes::gl3
{
	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eClearTexDepth >
	{
		static Op constexpr value = { OpType::eClearTexDepth, 6u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearTexDepth >
	{
		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, uint32_t format
			, uint32_t type
			, float depth )
			: cmd{ { OpType::eClearTexDepth, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, format{ std::move( format ) }
			, type{ std::move( type ) }
			, depth{ std::move( depth ) }
		{
		}

		Command cmd;
		uint32_t name;
		uint32_t mipLevel;
		uint32_t format;
		uint32_t type;
		float depth;
	};
	using CmdClearTexDepth = CmdT< OpType::eClearTexDepth >;

	void apply( ContextLock const & context
		, CmdClearTexDepth const & cmd );
	
	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eClearTexStencil >
	{
		static Op constexpr value = { OpType::eClearTexStencil, 6u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearTexStencil >
	{
		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, uint32_t format
			, uint32_t type
			, int32_t stencil )
			: cmd{ { OpType::eClearTexStencil, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, format{ std::move( format ) }
			, type{ std::move( type ) }
			, stencil{ std::move( stencil ) }
		{
		}

		Command cmd;
		uint32_t name;
		uint32_t mipLevel;
		uint32_t format;
		uint32_t type;
		int32_t stencil;
	};
	using CmdClearTexStencil = CmdT< OpType::eClearTexStencil >;

	void apply( ContextLock const & context
		, CmdClearTexStencil const & cmd );
	
	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eClearTexDepthStencil >
	{
		static Op constexpr value = { OpType::eClearTexDepthStencil, 7u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearTexDepthStencil >
	{
		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, uint32_t format
			, uint32_t type
			, VkClearDepthStencilValue depthStencil )
			: cmd{ { OpType::eClearTexDepthStencil, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, format{ std::move( format ) }
			, type{ std::move( type ) }
			, depthStencil{ std::move( depthStencil ) }
		{
		}

		Command cmd;
		uint32_t name;
		uint32_t mipLevel;
		uint32_t format;
		uint32_t type;
		VkClearDepthStencilValue depthStencil;
	};
	using CmdClearTexDepthStencil = CmdT< OpType::eClearTexDepthStencil >;

	void apply( ContextLock const & context
		, CmdClearTexDepthStencil const & cmd );

	//*************************************************************************

	void buildClearDepthStencilCommand( VkDevice device
		, VkImage image
		, VkImageLayout imageLayout
		, VkClearDepthStencilValue value
		, VkImageSubresourceRangeArray ranges
		, CmdList & list );

	//*************************************************************************
}
