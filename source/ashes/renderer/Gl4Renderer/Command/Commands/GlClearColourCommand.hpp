/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl4Renderer/Command/Commands/GlCommandBase.hpp"

#include "renderer/Gl4Renderer/Enum/GlFormat.hpp"

namespace ashes::gl4
{
	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eClearTexColor >
	{
		static Op constexpr value = { OpType::eClearTexColor, 9u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearTexColor >
	{
		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, uint32_t format
			, uint32_t type
			, VkClearColorValue color )
			: cmd{ { OpType::eClearTexColor, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, format{ std::move( format ) }
			, type{ std::move( type ) }
			, color{ std::move( color ) }
		{
		}

		Command cmd;
		uint32_t name;
		uint32_t mipLevel;
		uint32_t format;
		uint32_t type;
		VkClearColorValue color;
	};
	using CmdClearTexColor = CmdT< OpType::eClearTexColor >;

	void apply( ContextLock const & context
		, CmdClearTexColor const & cmd );

	//*************************************************************************

	void buildClearColourCommand( VkDevice device
		, VkImage image
		, VkImageLayout imageLayout
		, VkClearColorValue value
		, VkImageSubresourceRangeArray ranges
		, CmdList & list );

	//*************************************************************************
}
