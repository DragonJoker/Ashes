/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlScissorCommand.hpp"

#include "Core/GlDevice.hpp"

#include "ashesgl3_api.hpp"

using ashes::operator==;
using ashes::operator!=;

namespace ashes::gl3
{
	void buildScissorCommand( ContextStateStack & stack
		, uint32_t firstScissor
		, VkScissorArray scissors
		, CmdList & list )
	{
		glLogCommand( "ScissorCommand" );
		stack.apply( list, firstScissor, scissors, false );
	}
}
