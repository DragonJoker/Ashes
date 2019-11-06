/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl3Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl3
{
	void buildBeginSubpassCommand( VkRenderPass renderPass
		, VkFramebuffer frameBuffer
		, VkSubpassDescription subpass
		, CmdList & list );
}
