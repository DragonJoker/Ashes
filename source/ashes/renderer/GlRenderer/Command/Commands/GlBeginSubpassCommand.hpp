/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl
{
	void buildBeginSubpassCommand( ContextStateStack & stack
		, VkRenderPass renderPass
		, VkFramebuffer frameBuffer
		, VkSubpassDescription subpass
		, CmdList & list );
}
