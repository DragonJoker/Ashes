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
	struct CmdConfig< OpType::eCopyImageSubData >
	{
		static Op constexpr value = { OpType::eCopyImageSubData, 22u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eCopyImageSubData >
	{
		inline CmdT( uint32_t srcName
			, uint32_t srcTarget
			, uint32_t dstName
			, uint32_t dstTarget
			, VkImageCopy copy )
			: cmd{ { OpType::eCopyImageSubData, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, srcName{ std::move( srcName ) }
			, srcTarget{ std::move( srcTarget ) }
			, dstName{ std::move( dstName ) }
			, dstTarget{ std::move( dstTarget ) }
			, copy{ std::move( copy ) }
		{
		}

		Command cmd;
		uint32_t srcName;
		uint32_t srcTarget;
		uint32_t dstName;
		uint32_t dstTarget;
		VkImageCopy copy;
	};
	using CmdCopyImageSubData = CmdT< OpType::eCopyImageSubData >;

	void apply( ContextLock const & context
		, CmdCopyImageSubData const & cmd );

	//*************************************************************************

	void buildCopyImageCommand( VkImageCopy copyInfo
		, VkImage src
		, VkImage dst
		, CmdList & list );

	//*************************************************************************
}
