/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

#include "renderer/GlRenderer/Enum/GlFormat.hpp"

namespace ashes::gl
{
	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearTexColorF >
	{
		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, GlFormat format
			, std::array< float, 4u > color )
			: cmd{ { OpType::eClearTexColorF, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, format{ std::move( format ) }
			, color{ std::move( color ) }
		{
		}

		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, GlFormat format
			, float const ( & color )[4] )
			: cmd{ { OpType::eClearTexColorF, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, format{ std::move( format ) }
			, color{ color[0], color[1], color[2], color[3] }
		{
		}

		Command cmd;
		uint32_t name;
		uint32_t mipLevel;
		GlFormat format;
		std::array< float, 4u > color;
	};
	using CmdClearTexColorF = CmdT< OpType::eClearTexColorF >;

	void apply( ContextLock const & context
		, CmdClearTexColorF const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearTexColorSI >
	{
		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, GlFormat format
			, std::array< int32_t, 4u > color )
			: cmd{ { OpType::eClearTexColorSI, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, format{ std::move( format ) }
			, color{ std::move( color ) }
		{
		}

		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, GlFormat format
			, int32_t const ( & color )[4] )
			: cmd{ { OpType::eClearTexColorSI, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, format{ std::move( format ) }
			, color{ color[0], color[1], color[2], color[3] }
		{
		}

		Command cmd;
		uint32_t name;
		uint32_t mipLevel;
		GlFormat format;
		std::array< int32_t, 4u > color;
	};
	using CmdClearTexColorSI = CmdT< OpType::eClearTexColorSI >;

	void apply( ContextLock const & context
		, CmdClearTexColorSI const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearTexColorUI >
	{
		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, GlFormat format
			, std::array< uint32_t, 4u > color )
			: cmd{ { OpType::eClearTexColorUI, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, format{ std::move( format ) }
			, color{ std::move( color ) }
		{
		}

		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, GlFormat format
			, uint32_t const ( & color )[4] )
			: cmd{ { OpType::eClearTexColorUI, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, format{ std::move( format ) }
			, color{ color[0], color[1], color[2], color[3] }
		{
		}

		Command cmd;
		uint32_t name;
		uint32_t mipLevel;
		GlFormat format;
		std::array< uint32_t, 4u > color;
	};
	using CmdClearTexColorUI = CmdT< OpType::eClearTexColorUI >;

	void apply( ContextLock const & context
		, CmdClearTexColorUI const & cmd );

	//*************************************************************************

	void buildClearColourCommand( VkDevice device
		, ContextStateStack & stack
		, VkImage image
		, VkImageLayout imageLayout
		, VkClearColorValue value
		, ArrayView< VkImageSubresourceRange const > ranges
		, CmdList & list );

	//*************************************************************************
}
