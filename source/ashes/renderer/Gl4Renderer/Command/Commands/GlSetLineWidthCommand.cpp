/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlSetLineWidthCommand.hpp"

#include "Core/GlContextLock.hpp"

namespace ashes::gl4
{
	void apply( ContextLock const & context
		, CmdSetLineWidth const & cmd )
	{
		glLogCall( context
			, glLineWidth
			, cmd.width );
	}

	void buildSetLineWidthCommand( float width
		, CmdList & list )
	{
		glLogCommand( "SetLineWidthCommand" );
		list.push_back( makeCmd< OpType::eSetLineWidth >( width ) );
	}
}
