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
	struct alignas( uint64_t ) CmdT< OpType::eGenerateMipmaps >
	{
		inline CmdT( uint32_t target )
			: cmd{ { OpType::eGenerateMipmaps, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
		{
		}

		Command cmd;
		uint32_t target;
	};
	using CmdGenerateMipmaps = CmdT< OpType::eGenerateMipmaps >;

	void apply( ContextLock const & context
		, CmdGenerateMipmaps const & cmd );

	//*************************************************************************

	void buildGenerateMipmapsCommand( VkImage texture
		, CmdList & list );

	//*************************************************************************
}
