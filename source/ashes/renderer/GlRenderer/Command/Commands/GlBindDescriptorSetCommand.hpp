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
	struct alignas( uint64_t ) CmdT< OpType::eTexParameteri >
	{
		inline CmdT( GlTextureType target
			, uint32_t name
			, int32_t param )
			: cmd{ { OpType::eTexParameteri, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
			, name{ std::move( name ) }
			, param{ std::move( param ) }
		{
		}

		Command cmd;
		GlTextureType target;
		uint32_t name;
		int32_t param;
	};
	using CmdTexParameteri = CmdT< OpType::eTexParameteri >;

	void apply( ContextLock const & context
		, CmdTexParameteri const & cmd );
	
	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eTexParameterf >
	{
		inline CmdT( GlTextureType target
			, uint32_t name
			, float param )
			: cmd{ { OpType::eTexParameterf, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
			, name{ std::move( name ) }
			, param{ std::move( param ) }
		{
		}

		Command cmd;
		GlTextureType target;
		uint32_t name;
		float param;
	};
	using CmdTexParameterf = CmdT< OpType::eTexParameterf >;

	void apply( ContextLock const & context
		, CmdTexParameterf const & cmd );

	//*************************************************************************

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

	void buildBindDescriptorSetCommand( VkDevice device
		, VkDescriptorSet descriptorSet
		, uint32_t descriptorSetIndex
		, VkPipelineLayout pipelineLayout
		, ArrayView< uint32_t const > const & dynamicOffsets
		, uint32_t & dynamicOffsetIndex
		, VkPipelineBindPoint bindingPoint
		, CmdList & list );

	//*************************************************************************
}
