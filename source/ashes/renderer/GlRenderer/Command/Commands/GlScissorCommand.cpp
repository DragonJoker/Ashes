/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlScissorCommand.hpp"

#include "Core/GlDevice.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	void buildScissorCommand( ContextStateStack & stack
		, uint32_t firstScissor
		, ArrayView< VkRect2D const > scissors
		, CmdList & list
		, PreExecuteActions & preExecuteActions )
	{
		glLogCommand( list, "ScissorCommand" );
		stack.apply( list, preExecuteActions, firstScissor, scissors, false );
	}
}
