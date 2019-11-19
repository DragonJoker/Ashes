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

	template<>
	struct CmdConfig< OpType::eGetTexImage >
	{
		static Op constexpr value = { OpType::eGetTexImage, 5u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eGetTexImage >
	{
		inline CmdT( uint32_t target
			, GlFormat format
			, GlType type )
			: cmd{ { OpType::eGetTexImage, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
			, format{ std::move( format ) }
			, type{ std::move( type ) }
		{
		}

		Command cmd;
		uint32_t target;
		GlFormat format;
		GlType type;
	};
	using CmdGetTexImage = CmdT< OpType::eGetTexImage >;

	void apply( ContextLock const & context
		, CmdGetTexImage const & cmd );

	//*************************************************************************

	void buildCopyImageCommand( VkImageCopy copyInfo
		, VkImage src
		, VkImage dst
		, CmdList & list );

	//*************************************************************************
}
