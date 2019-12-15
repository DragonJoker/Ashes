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
	struct CmdConfig< OpType::eClearBack >
	{
		static Op constexpr value = { OpType::eClearBack, 2u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearBack >
	{
		inline CmdT( uint32_t mask )
			: cmd{ { OpType::eClearBack, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, mask{ std::move( mask ) }
		{
		}

		Command cmd;
		uint32_t mask;
	};
	using CmdClearBack = CmdT< OpType::eClearBack >;

	void apply( ContextLock const & context
		, CmdClearBack const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eClearBackColour >
	{
		static Op constexpr value = { OpType::eClearBackColour, 6u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearBackColour >
	{
		inline CmdT( VkClearColorValue color
			, uint32_t colourIndex )
			: cmd{ { OpType::eClearBackColour, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, color{ std::move( color ) }
			, colourIndex{ std::move( colourIndex ) }
		{
		}

		Command cmd;
		VkClearColorValue color;
		uint32_t colourIndex;
	};
	using CmdClearBackColour = CmdT< OpType::eClearBackColour >;

	void apply( ContextLock const & context
		, CmdClearBackColour const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eClearBackDepth >
	{
		static Op constexpr value = { OpType::eClearBackDepth, 2u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearBackDepth >
	{
		inline CmdT( float depth )
			: cmd{ { OpType::eClearBackDepth, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, depth{ std::move( depth ) }
		{
		}

		Command cmd;
		float depth;
	};
	using CmdClearBackDepth = CmdT< OpType::eClearBackDepth >;

	void apply( ContextLock const & context
		, CmdClearBackDepth const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eClearBackStencil >
	{
		static Op constexpr value = { OpType::eClearBackStencil, 2u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearBackStencil >
	{
		inline CmdT( int32_t stencil )
			: cmd{ { OpType::eClearBackStencil, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, stencil{ std::move( stencil ) }
		{
		}

		Command cmd;
		int32_t stencil;
	};
	using CmdClearBackStencil = CmdT< OpType::eClearBackStencil >;

	void apply( ContextLock const & context
		, CmdClearBackStencil const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eClearBackDepthStencil >
	{
		static Op constexpr value = { OpType::eClearBackDepthStencil, 3u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearBackDepthStencil >
	{
		inline CmdT( float depth
			, int32_t stencil )
			: cmd{ { OpType::eClearBackDepthStencil, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, depth{ std::move( depth ) }
			, stencil{ std::move( stencil ) }
		{
		}

		Command cmd;
		float depth;
		int32_t stencil;
	};
	using CmdClearBackDepthStencil = CmdT< OpType::eClearBackDepthStencil >;

	void apply( ContextLock const & context
		, CmdClearBackDepthStencil const & cmd );

	//*************************************************************************

	void buildBeginRenderPassCommand( ContextStateStack & stack
		, VkRenderPass renderPass
		, VkFramebuffer frameBuffer
		, VkClearValueArray clearValues
		, VkSubpassContents contents
		, CmdList & list
		, PreExecuteActions & preExecuteActions );

	//*************************************************************************
}
