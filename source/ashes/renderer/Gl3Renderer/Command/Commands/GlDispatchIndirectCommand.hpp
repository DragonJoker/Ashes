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
	struct CmdConfig< OpType::eDispatchIndirect >
	{
		static Op constexpr value = { OpType::eDispatchIndirect, 3u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDispatchIndirect >
	{
		inline CmdT( uint64_t offset )
			: cmd{ { OpType::eDispatchIndirect, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, offset{ std::move( offset ) }
		{
		}

		Command cmd;
		uint64_t offset;
	};
	using CmdDispatchIndirect = CmdT< OpType::eDispatchIndirect >;

	void apply( ContextLock const & context
		, CmdDispatchIndirect const & cmd );

	//*************************************************************************

	void buildDispatchIndirectCommand( VkBuffer buffer
		, VkDeviceSize offset
		, CmdList & list );

	//*************************************************************************
}
