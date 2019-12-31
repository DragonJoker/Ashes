/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl
{
	void buildScissorCommand( ContextStateStack & stack
		, uint32_t firstScissor
		, VkScissorArray scissors
		, CmdList & list
		, PreExecuteActions & preExecuteActions );
}
