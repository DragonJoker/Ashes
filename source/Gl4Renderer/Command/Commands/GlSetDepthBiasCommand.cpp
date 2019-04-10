/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlSetDepthBiasCommand.hpp"

namespace ashes::gl4
{
	void apply( ContextLock const & context
		, CmdSetDepthBias const & cmd )
	{
		glLogCall( context
			, glPolygonOffsetClampEXT
			, cmd.slopeFactor
			, cmd.constantFactor
			, cmd.clamp );
	}

	void buildSetDepthBiasCommand( float constantFactor
		, float clamp
		, float slopeFactor
		, CmdList & list )
	{
		glLogCommand( "SetDepthBiasCommand" );
		list.push_back( makeCmd< OpType::eSetDepthBias >( constantFactor
			, clamp
			, slopeFactor ) );
	}
}
