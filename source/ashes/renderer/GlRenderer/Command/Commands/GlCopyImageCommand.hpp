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
	struct CmdConfig< OpType::eCopyImageSubData >
	{
		static Op constexpr value = { OpType::eCopyImageSubData, 22u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eCopyImageSubData >
	{
		inline CmdT( uint32_t srcName
			, GlTextureType srcTarget
			, uint32_t dstName
			, GlTextureType dstTarget
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
		GlTextureType srcTarget;
		uint32_t dstName;
		GlTextureType dstTarget;
		VkImageCopy copy;
	};
	using CmdCopyImageSubData = CmdT< OpType::eCopyImageSubData >;

	void apply( ContextLock const & context
		, CmdCopyImageSubData const & cmd );

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
			, GlTextureType srcTarget
			, VkImage dst
			, uint32_t dstName
			, GlTextureType dstTarget
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
		GlTextureType srcTarget;
		VkImage dst;
		uint32_t dstName;
		GlTextureType dstTarget;
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
			, GlTextureType srcTarget
			, VkImage dst
			, uint32_t dstName
			, GlTextureType dstTarget
			, GlTextureType dstLayerTarget
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
		GlTextureType srcTarget;
		VkImage dst;
		uint32_t dstName;
		GlTextureType dstTarget;
		GlTextureType dstLayerTarget;
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
			, GlTextureType srcTarget
			, VkImage dst
			, uint32_t dstName
			, GlTextureType dstTarget
			, GlTextureType dstLayerTarget
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
		GlTextureType srcTarget;
		VkImage dst;
		uint32_t dstName;
		GlTextureType dstTarget;
		GlTextureType dstLayerTarget;
		int32_t dstOffsetZ;
		uint32_t dstExtentZ;
		VkImageCopy copy;
	};
	using CmdCopyImageSubData3D = CmdT< OpType::eCopyImageSubData3D >;

	void apply( ContextLock const & context
		, CmdCopyImageSubData3D const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eGetTexImage >
	{
		static Op constexpr value = { OpType::eGetTexImage, 5u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eGetTexImage >
	{
		inline CmdT( GlTextureType target
			, GlFormat format
			, GlType type )
			: cmd{ { OpType::eGetTexImage, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
			, format{ std::move( format ) }
			, type{ std::move( type ) }
		{
		}

		Command cmd;
		GlTextureType target;
		GlFormat format;
		GlType type;
	};
	using CmdGetTexImage = CmdT< OpType::eGetTexImage >;

	void apply( ContextLock const & context
		, CmdGetTexImage const & cmd );

	//*************************************************************************

	void buildCopyImageCommand( ContextStateStack & stack
		, VkDevice device
		, VkImageCopy copyInfo
		, VkImage srcImage
		, VkImage dstImage
		, CmdList & list
		, VkImageViewArray & views );

	//*************************************************************************
}
