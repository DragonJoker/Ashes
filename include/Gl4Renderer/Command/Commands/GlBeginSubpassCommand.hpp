/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
{
	void buildBeginSubpassCommand( VkRenderPass renderPass
		, VkFramebuffer frameBuffer
		, VkSubpassDescription subpass
		, CmdList & list );
}
