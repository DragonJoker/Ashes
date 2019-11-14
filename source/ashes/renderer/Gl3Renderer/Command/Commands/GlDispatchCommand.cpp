/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlDispatchCommand.hpp"

#include "Core/GlContextLock.hpp"

namespace ashes::gl3
{
	void apply( ContextLock const & context
		, CmdDispatch const & cmd )
	{
		glLogCall( context
			, glDispatchCompute_ARB
			, cmd.groupCountX
			, cmd.groupCountY
			, cmd.groupCountZ );
	}

	void buildDispatchCommand( uint32_t groupCountX
		, uint32_t groupCountY
		, uint32_t groupCountZ
		, CmdList & list )
	{
		glLogCommand( "DispatchCommand" );
		list.push_back( makeCmd< OpType::eDispatch >( groupCountX
			, groupCountY
			, groupCountZ ) );
	}
}
