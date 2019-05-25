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
	struct CmdConfig< OpType::eReadPixels >
	{
		static Op constexpr value = { OpType::eReadPixels, 7u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eReadPixels >
	{
		inline CmdT( int32_t x
			, int32_t y
			, uint32_t width
			, uint32_t height
			, uint32_t format
			, uint32_t type )
			: cmd{ { OpType::eReadPixels, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, x{ std::move( x ) }
			, y{ std::move( y ) }
			, width{ std::move( width ) }
			, height{ std::move( height ) }
			, format{ std::move( format ) }
			, type{ std::move( type ) }
		{
		}

		Command cmd;
		int32_t x;
		int32_t y;
		uint32_t width;
		uint32_t height;
		uint32_t format;
		uint32_t type;
	};
	using CmdReadPixels = CmdT< OpType::eReadPixels >;

	void apply( ContextLock const & context
		, CmdReadPixels const & cmd );

	//*************************************************************************

	void buildCopyImageToBufferCommand( VkDevice device
		, VkBufferImageCopy copyInfo
		, VkImage src
		, VkBuffer dst
		, CmdList & list );

	//*************************************************************************
}
