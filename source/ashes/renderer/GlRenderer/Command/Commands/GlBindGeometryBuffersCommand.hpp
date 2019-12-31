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
	struct CmdConfig< OpType::eBindVextexArrayObject >
	{
		static Op constexpr value = { OpType::eBindVextexArrayObject, 4u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindVextexArrayObject >
	{
		inline CmdT( GLuint vao )
			: cmd{ { OpType::eBindVextexArrayObject, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, vao{ std::move( vao ) }
		{
		}

		Command cmd;
		GLuint vao;
	};
	using CmdBindVextexArrayObject = CmdT< OpType::eBindVextexArrayObject >;

	void apply( ContextLock const & context
		, CmdBindVextexArrayObject const & cmd );
	
	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eBindVextexArray >
	{
		static Op constexpr value = { OpType::eBindVextexArray, 4u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindVextexArray >
	{
		inline CmdT( GeometryBuffers const * vao )
			: cmd{ { OpType::eBindVextexArray, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, vao{ std::move( vao ) }
		{
		}

		Command cmd;
		GeometryBuffers const * vao;
	};
	using CmdBindVextexArray = CmdT< OpType::eBindVextexArray >;

	void apply( ContextLock const & context
		, CmdBindVextexArray const & cmd );

	//*************************************************************************

	void buildBindGeometryBuffersCommand( GeometryBuffers const & vao
		, CmdList & list );

	//*************************************************************************
}
