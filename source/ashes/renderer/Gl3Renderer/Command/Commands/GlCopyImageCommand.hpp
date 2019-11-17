/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl3Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl3
{
	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eCopyImageSubData1D >
	{
		static Op constexpr value = { OpType::eCopyImageSubData1D, 22u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eCopyImageSubData1D >
	{
		inline CmdT( VkImage src
			, uint32_t srcName
			, uint32_t srcTarget
			, VkImage dst
			, uint32_t dstName
			, uint32_t dstTarget
			, VkImageCopy copy )
			: cmd{ { OpType::eCopyImageSubData1D, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, src{ std::move( src ) }
			, srcName{ std::move( srcName ) }
			, srcTarget{ std::move( srcTarget ) }
			, dst{ std::move( dst ) }
			, dstName{ std::move( dstName ) }
			, dstTarget{ std::move( dstTarget ) }
			, copy{ std::move( copy ) }
		{
		}

		Command cmd;
		VkImage src;
		uint32_t srcName;
		uint32_t srcTarget;
		VkImage dst;
		uint32_t dstName;
		uint32_t dstTarget;
		VkImageCopy copy;
	};
	using CmdCopyImageSubData1D = CmdT< OpType::eCopyImageSubData1D >;

	void apply( ContextLock const & context
		, CmdCopyImageSubData1D const & cmd );
	
	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eCopyImageSubData2D >
	{
		static Op constexpr value = { OpType::eCopyImageSubData2D, 22u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eCopyImageSubData2D >
	{
		inline CmdT( VkImage src
			, uint32_t srcName
			, uint32_t srcTarget
			, VkImage dst
			, uint32_t dstName
			, uint32_t dstTarget
			, uint32_t dstLayerTarget
			, int32_t dstOffsetY
			, uint32_t dstExtentY
			, VkImageCopy copy )
			: cmd{ { OpType::eCopyImageSubData2D, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, src{ std::move( src ) }
			, srcName{ std::move( srcName ) }
			, srcTarget{ std::move( srcTarget ) }
			, dst{ std::move( dst ) }
			, dstName{ std::move( dstName ) }
			, dstTarget{ std::move( dstTarget ) }
			, dstLayerTarget{ std::move( dstLayerTarget ) }
			, dstOffsetY{ std::move( dstOffsetY ) }
			, dstExtentY{ std::move( dstExtentY ) }
			, copy{ std::move( copy ) }
		{
		}

		Command cmd;
		VkImage src;
		uint32_t srcName;
		uint32_t srcTarget;
		VkImage dst;
		uint32_t dstName;
		uint32_t dstTarget;
		uint32_t dstLayerTarget;
		int32_t dstOffsetY;
		uint32_t dstExtentY;
		VkImageCopy copy;
	};
	using CmdCopyImageSubData2D = CmdT< OpType::eCopyImageSubData2D >;

	void apply( ContextLock const & context
		, CmdCopyImageSubData2D const & cmd );
	
	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eCopyImageSubData3D >
	{
		static Op constexpr value = { OpType::eCopyImageSubData3D, 22u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eCopyImageSubData3D >
	{
		inline CmdT( VkImage src
			, uint32_t srcName
			, uint32_t srcTarget
			, VkImage dst
			, uint32_t dstName
			, uint32_t dstTarget
			, uint32_t dstLayerTarget
			, int32_t dstOffsetZ
			, uint32_t dstExtentZ
			, VkImageCopy copy )
			: cmd{ { OpType::eCopyImageSubData3D, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, src{ std::move( src ) }
			, srcName{ std::move( srcName ) }
			, srcTarget{ std::move( srcTarget ) }
			, dst{ std::move( dst ) }
			, dstName{ std::move( dstName ) }
			, dstTarget{ std::move( dstTarget ) }
			, dstLayerTarget{ std::move( dstLayerTarget ) }
			, dstOffsetZ{ std::move( dstOffsetZ ) }
			, dstExtentZ{ std::move( dstExtentZ ) }
			, copy{ std::move( copy ) }
		{
		}

		Command cmd;
		VkImage src;
		uint32_t srcName;
		uint32_t srcTarget;
		VkImage dst;
		uint32_t dstName;
		uint32_t dstTarget;
		uint32_t dstLayerTarget;
		int32_t dstOffsetZ;
		uint32_t dstExtentZ;
		VkImageCopy copy;
	};
	using CmdCopyImageSubData3D = CmdT< OpType::eCopyImageSubData3D >;

	void apply( ContextLock const & context
		, CmdCopyImageSubData3D const & cmd );

	//*************************************************************************

	void buildCopyImageCommand( VkImageCopy copyInfo
		, VkImage src
		, VkImage dst
		, CmdList & list );

	//*************************************************************************
}
