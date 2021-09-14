/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlSetLineWidthCommand.hpp"

#include "Core/GlContextLock.hpp"

namespace ashes::gl
{
	void buildSetLineWidthCommand( float width
		, CmdList & list )
	{
		glLogCommand( list, "SetLineWidthCommand" );
		list.push_back( makeCmd< OpType::eSetLineWidth >( width ) );
	}
}
