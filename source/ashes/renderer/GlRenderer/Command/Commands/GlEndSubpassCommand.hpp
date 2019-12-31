/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl
{
	void buildEndSubpassCommand( VkDevice device
		, ContextStateStack & stack
		, VkFramebuffer frameBuffer
		, VkSubpassDescription const & subpass
		, CmdList & list
		, PreExecuteActions & preExecuteActions );
}
