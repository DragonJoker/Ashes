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
	struct CmdConfig< OpType::eCopyBufferSubData >
	{
		static Op constexpr value = { OpType::eCopyBufferSubData, 5u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eCopyBufferSubData >
	{
		inline CmdT( uint32_t srcTarget
			, uint32_t dstTarget
			, VkBufferCopy copy )
			: cmd{ { OpType::eCopyBufferSubData, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, srcTarget{ GlBufferTarget( srcTarget ) }
			, dstTarget{ GlBufferTarget( dstTarget ) }
			, copy{ std::move( copy ) }
		{
		}

		Command cmd;
		GlBufferTarget srcTarget;
		GlBufferTarget dstTarget;
		VkBufferCopy copy;
	};
	using CmdCopyBufferSubData = CmdT< OpType::eCopyBufferSubData >;

	void apply( ContextLock const & context
		, CmdCopyBufferSubData const & cmd );

	//*************************************************************************

	void buildCopyBufferCommand( VkBufferCopy copyInfo
		, VkBuffer src
		, VkBuffer dst
		, CmdList & list );

	//*************************************************************************
}
