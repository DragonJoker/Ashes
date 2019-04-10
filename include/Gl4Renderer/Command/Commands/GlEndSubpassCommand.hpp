/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
{
	void buildEndSubpassCommand( VkFramebuffer frameBuffer
		, VkSubpassDescription const & subpass
		, CmdList & list );
}
