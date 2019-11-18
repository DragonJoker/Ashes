/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl3Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl3
{
	void buildEndSubpassCommand( VkDevice device
		, ContextStateStack & stack
		, VkFramebuffer frameBuffer
		, VkSubpassDescription const & subpass
		, CmdList & list );
}
