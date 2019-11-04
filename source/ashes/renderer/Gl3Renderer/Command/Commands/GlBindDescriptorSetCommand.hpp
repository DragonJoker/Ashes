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
	struct CmdConfig< OpType::eBindSampler >
	{
		static Op constexpr value = { OpType::eBindSampler, 3u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindSampler >
	{
		inline CmdT( uint32_t binding
			, uint32_t name )
			: cmd{ { OpType::eBindSampler, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, binding{ std::move( binding ) }
			, name{ std::move( name ) }
		{
		}

		Command cmd;
		uint32_t binding;
		uint32_t name;
	};
	using CmdBindSampler = CmdT< OpType::eBindSampler >;

	void apply( ContextLock const & context
		, CmdBindSampler const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eTexParameteri >
	{
		static Op constexpr value = { OpType::eTexParameteri, 4u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eTexParameteri >
	{
		inline CmdT( uint32_t target
			, uint32_t name
			, int32_t param )
			: cmd{ { OpType::eTexParameteri, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
			, name{ std::move( name ) }
			, param{ std::move( param ) }
		{
		}

		Command cmd;
		uint32_t target;
		uint32_t name;
		int32_t param;
	};
	using CmdTexParameteri = CmdT< OpType::eTexParameteri >;

	void apply( ContextLock const & context
		, CmdTexParameteri const & cmd );
	
	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eTexParameterf >
	{
		static Op constexpr value = { OpType::eTexParameterf, 4u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eTexParameterf >
	{
		inline CmdT( uint32_t target
			, uint32_t name
			, float param )
			: cmd{ { OpType::eTexParameterf, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
			, name{ std::move( name ) }
			, param{ std::move( param ) }
		{
		}

		Command cmd;
		uint32_t target;
		uint32_t name;
		float param;
	};
	using CmdTexParameterf = CmdT< OpType::eTexParameterf >;

	void apply( ContextLock const & context
		, CmdTexParameterf const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eBindImage >
	{
		static Op constexpr value = { OpType::eBindImage, 7u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindImage >
	{
		inline CmdT( uint32_t binding
			, uint32_t name
			, uint32_t baseMipLevel
			, uint32_t layerCount
			, uint32_t baseArrayLayer
			, GlInternal internal )
			: cmd{ { OpType::eBindImage, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, binding{ std::move( binding ) }
			, name{ std::move( name ) }
			, baseMipLevel{ std::move( baseMipLevel ) }
			, layerCount{ std::move( layerCount ) }
			, baseArrayLayer{ std::move( baseArrayLayer ) }
			, internal{ std::move( internal ) }
		{
		}

		Command cmd;
		uint32_t binding;
		uint32_t name;
		uint32_t baseMipLevel;
		uint32_t layerCount;
		uint32_t baseArrayLayer;
		GlInternal internal;
	};
	using CmdBindImage = CmdT< OpType::eBindImage >;

	void apply( ContextLock const & context
		, CmdBindImage const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eBindBufferRange >
	{
		static Op constexpr value = { OpType::eBindBufferRange, 6u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindBufferRange >
	{
		inline CmdT( uint32_t binding
			, uint32_t target
			, uint32_t name
			, int64_t offset
			, int64_t range )
			: cmd{ { OpType::eBindBufferRange, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, binding{ std::move( binding ) }
			, target{ GlBufferTarget( target ) }
			, name{ std::move( name ) }
			, offset{ std::move( offset ) }
			, range{ std::move( range ) }
		{
		}

		Command cmd;
		uint32_t binding;
		GlBufferTarget target;
		uint32_t name;
		int64_t offset;
		int64_t range;
	};
	using CmdBindBufferRange = CmdT< OpType::eBindBufferRange >;

	void apply( ContextLock const & context
		, CmdBindBufferRange const & cmd );

	//*************************************************************************

	void buildBindDescriptorSetCommand( VkDescriptorSet descriptorSet
		, VkPipelineLayout layout
		, UInt32Array const & dynamicOffsets
		, VkPipelineBindPoint bindingPoint
		, CmdList & list );

	//*************************************************************************
}
