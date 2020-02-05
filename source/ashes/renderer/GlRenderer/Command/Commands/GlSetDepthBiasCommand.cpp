/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlSetDepthBiasCommand.hpp"

#include "Core/GlContextLock.hpp"

namespace ashes::gl
{
	void buildSetDepthBiasCommand( float constantFactor
		, float clamp
		, float slopeFactor
		, CmdList & list )
	{
		glLogCommand( list, "SetDepthBiasCommand" );
		list.push_back( makeCmd< OpType::ePolygonOffset >( constantFactor
			, clamp
			, slopeFactor ) );
	}
}
