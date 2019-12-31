/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl
{
	void buildViewportCommand( ContextStateStack & stack
		, uint32_t firstViewport
		, VkViewportArray viewports
		, CmdList & list
		, PreExecuteActions & preExecuteActions );
}
