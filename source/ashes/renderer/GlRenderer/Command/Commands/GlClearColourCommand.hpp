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
	struct CmdConfig< OpType::eClearTexColor >
	{
		static Op constexpr value = { OpType::eClearTexColor, 9u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearTexColor >
	{
		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, std::array< float, 4u > color )
			: cmd{ { OpType::eClearTexColor, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, color{ std::move( color ) }
		{
		}

		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, float const ( & color )[4] )
			: cmd{ { OpType::eClearTexColor, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, color{ color[0], color[1], color[2], color[3] }
		{
		}

		Command cmd;
		uint32_t name;
		uint32_t mipLevel;
		std::array< float, 4u > color;
	};
	using CmdClearTexColor = CmdT< OpType::eClearTexColor >;

	void apply( ContextLock const & context
		, CmdClearTexColor const & cmd );

	//*************************************************************************

	void buildClearColourCommand( VkDevice device
		, VkImage image
		, VkImageLayout imageLayout
		, VkClearColorValue value
		, ArrayView< VkImageSubresourceRange const > ranges
		, CmdList & list );

	//*************************************************************************
}
