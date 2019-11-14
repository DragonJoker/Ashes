/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl3Renderer/Command/Commands/GlCommandBase.hpp"

#include "renderer/Gl3Renderer/Enum/GlFormat.hpp"
#include "renderer/Gl3Renderer/Enum/GlTextureType.hpp"

namespace ashes::gl3
{
	//*************************************************************************

	template<>
	struct CmdConfig< OpType::ePixelStore >
	{
		static Op constexpr value = { OpType::ePixelStore, 3u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::ePixelStore >
	{
		inline CmdT( uint32_t name
			, int32_t param )
			: cmd{ { OpType::ePixelStore, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, param{ std::move( param ) }
		{
		}

		Command cmd;
		uint32_t name;
		int32_t param;
	};
	using CmdPixelStore = CmdT< OpType::ePixelStore >;

	void apply( ContextLock const & context
		, CmdPixelStore const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eCompressedTexSubImage1D >
	{
		static Op constexpr value = { OpType::eCompressedTexSubImage1D, 8u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eCompressedTexSubImage1D >
	{
		inline CmdT( uint32_t copyTarget
			, uint32_t mipLevel
			, int32_t x
			, uint32_t width
			, uint32_t format
			, int32_t imageSize
			, int32_t bufferOffset )
			: cmd{ { OpType::eCompressedTexSubImage1D, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, copyTarget{ std::move( copyTarget ) }
			, mipLevel{ std::move( mipLevel ) }
			, x{ std::move( x ) }
			, width{ std::move( width ) }
			, format{ std::move( format ) }
			, imageSize{ std::move( imageSize ) }
			, bufferOffset{ std::move( bufferOffset ) }
		{
		}

		Command cmd;
		uint32_t copyTarget;
		uint32_t mipLevel;
		int32_t x;
		uint32_t width;
		uint32_t format;
		int32_t imageSize;
		int32_t bufferOffset;
	};
	using CmdCompressedTexSubImage1D = CmdT< OpType::eCompressedTexSubImage1D >;

	void apply( ContextLock const & context
		, CmdCompressedTexSubImage1D const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eCompressedTexSubImage2D >
	{
		static Op constexpr value = { OpType::eCompressedTexSubImage2D, 10u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eCompressedTexSubImage2D >
	{
		inline CmdT( uint32_t copyTarget
			, uint32_t mipLevel
			, int32_t x
			, int32_t y
			, uint32_t width
			, uint32_t height
			, uint32_t format
			, int32_t imageSize
			, int32_t bufferOffset )
			: cmd{ { OpType::eCompressedTexSubImage2D, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, copyTarget{ std::move( copyTarget ) }
			, mipLevel{ std::move( mipLevel ) }
			, x{ std::move( x ) }
			, y{ std::move( y ) }
			, width{ std::move( width ) }
			, height{ std::move( height ) }
			, format{ std::move( format ) }
			, imageSize{ std::move( imageSize ) }
			, bufferOffset{ std::move( bufferOffset ) }
		{
		}

		Command cmd;
		uint32_t copyTarget;
		uint32_t mipLevel;
		int32_t x;
		int32_t y;
		uint32_t width;
		uint32_t height;
		uint32_t format;
		int32_t imageSize;
		int32_t bufferOffset;
	};
	using CmdCompressedTexSubImage2D = CmdT< OpType::eCompressedTexSubImage2D >;

	void apply( ContextLock const & context
		, CmdCompressedTexSubImage2D const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eCompressedTexSubImage3D >
	{
		static Op constexpr value = { OpType::eCompressedTexSubImage3D, 12u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eCompressedTexSubImage3D >
	{
		inline CmdT( uint32_t copyTarget
			, uint32_t mipLevel
			, int32_t x
			, int32_t y
			, int32_t z
			, uint32_t width
			, uint32_t height
			, uint32_t depth
			, uint32_t format
			, int32_t imageSize
			, int32_t bufferOffset )
			: cmd{ { OpType::eCompressedTexSubImage3D, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, copyTarget{ std::move( copyTarget ) }
			, mipLevel{ std::move( mipLevel ) }
			, x{ std::move( x ) }
			, y{ std::move( y ) }
			, z{ std::move( z ) }
			, width{ std::move( width ) }
			, height{ std::move( height ) }
			, depth{ std::move( depth ) }
			, format{ std::move( format ) }
			, imageSize{ std::move( imageSize ) }
			, bufferOffset{ std::move( bufferOffset ) }
		{
		}

		Command cmd;
		uint32_t copyTarget;
		uint32_t mipLevel;
		int32_t x;
		int32_t y;
		int32_t z;
		uint32_t width;
		uint32_t height;
		uint32_t depth;
		uint32_t format;
		int32_t imageSize;
		int32_t bufferOffset;
	};
	using CmdCompressedTexSubImage3D = CmdT< OpType::eCompressedTexSubImage3D >;

	void apply( ContextLock const & context
		, CmdCompressedTexSubImage3D const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eTexSubImage1D >
	{
		static Op constexpr value = { OpType::eTexSubImage1D, 8u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eTexSubImage1D >
	{
		inline CmdT( uint32_t copyTarget
			, uint32_t mipLevel
			, int32_t x
			, uint32_t width
			, uint32_t format
			, uint32_t type
			, int32_t bufferOffset )
			: cmd{ { OpType::eTexSubImage1D, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, copyTarget{ std::move( copyTarget ) }
			, mipLevel{ std::move( mipLevel ) }
			, x{ std::move( x ) }
			, width{ std::move( width ) }
			, format{ std::move( format ) }
			, type{ std::move( type ) }
			, bufferOffset{ std::move( bufferOffset ) }
		{
		}

		Command cmd;
		uint32_t copyTarget;
		uint32_t mipLevel;
		int32_t x;
		uint32_t width;
		uint32_t format;
		uint32_t type;
		int32_t bufferOffset;
	};
	using CmdTexSubImage1D = CmdT< OpType::eTexSubImage1D >;

	void apply( ContextLock const & context
		, CmdTexSubImage1D const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eTexSubImage2D >
	{
		static Op constexpr value = { OpType::eTexSubImage2D, 10u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eTexSubImage2D >
	{
		inline CmdT( uint32_t copyTarget
			, uint32_t mipLevel
			, int32_t x
			, int32_t y
			, uint32_t width
			, uint32_t height
			, uint32_t format
			, uint32_t type
			, int32_t bufferOffset )
			: cmd{ { OpType::eTexSubImage2D, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, copyTarget{ std::move( copyTarget ) }
			, mipLevel{ std::move( mipLevel ) }
			, x{ std::move( x ) }
			, y{ std::move( y ) }
			, width{ std::move( width ) }
			, height{ std::move( height ) }
			, format{ std::move( format ) }
			, type{ std::move( type ) }
			, bufferOffset{ std::move( bufferOffset ) }
		{
		}

		Command cmd;
		uint32_t copyTarget;
		uint32_t mipLevel;
		int32_t x;
		int32_t y;
		uint32_t width;
		uint32_t height;
		uint32_t format;
		uint32_t type;
		int32_t bufferOffset;
	};
	using CmdTexSubImage2D = CmdT< OpType::eTexSubImage2D >;

	void apply( ContextLock const & context
		, CmdTexSubImage2D const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eTexSubImage3D >
	{
		static Op constexpr value = { OpType::eTexSubImage3D, 12u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eTexSubImage3D >
	{
		inline CmdT( uint32_t copyTarget
			, uint32_t mipLevel
			, int32_t x
			, int32_t y
			, int32_t z
			, uint32_t width
			, uint32_t height
			, uint32_t depth
			, uint32_t format
			, uint32_t type
			, int32_t bufferOffset )
			: cmd{ { OpType::eTexSubImage3D, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, copyTarget{ std::move( copyTarget ) }
			, mipLevel{ std::move( mipLevel ) }
			, x{ std::move( x ) }
			, y{ std::move( y ) }
			, z{ std::move( z ) }
			, width{ std::move( width ) }
			, height{ std::move( height ) }
			, depth{ std::move( depth ) }
			, format{ std::move( format ) }
			, type{ std::move( type ) }
			, bufferOffset{ std::move( bufferOffset ) }
		{
		}

		Command cmd;
		uint32_t copyTarget;
		uint32_t mipLevel;
		int32_t x;
		int32_t y;
		int32_t z;
		uint32_t width;
		uint32_t height;
		uint32_t depth;
		uint32_t format;
		uint32_t type;
		int32_t bufferOffset;
	};
	using CmdTexSubImage3D = CmdT< OpType::eTexSubImage3D >;

	void apply( ContextLock const & context
		, CmdTexSubImage3D const & cmd );

	//*************************************************************************

	void buildCopyBufferToImageCommand( VkBufferImageCopy copyInfo
		, VkBuffer src
		, VkImage dst
		, CmdList & list );

	//*************************************************************************
}
