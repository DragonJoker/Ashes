/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlDispatchCommand.hpp"

#include "Core/GlContextLock.hpp"

namespace ashes::gl
{
	void buildDispatchCommand( uint32_t groupCountX
		, uint32_t groupCountY
		, uint32_t groupCountZ
		, CmdList & list )
	{
		glLogCommand( list, "DispatchCommand" );
		list.push_back( makeCmd< OpType::eDispatch >( groupCountX
			, groupCountY
			, groupCountZ ) );
	}
}
