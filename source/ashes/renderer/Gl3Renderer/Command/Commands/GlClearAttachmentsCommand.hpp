/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl3Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl3
{
	void buildClearAttachmentsCommand( ContextStateStack & stack
		, VkClearAttachmentArray clearAttaches
		, VkClearRectArray clearRects
		, CmdList & list );
}
