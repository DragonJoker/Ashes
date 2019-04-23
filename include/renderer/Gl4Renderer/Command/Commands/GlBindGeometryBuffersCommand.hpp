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
