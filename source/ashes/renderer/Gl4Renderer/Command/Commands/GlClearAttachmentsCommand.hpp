/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
{
	void buildClearAttachmentsCommand( ContextStateStack & stack
		, VkClearAttachmentArray clearAttaches
		, VkClearRectArray clearRects
		, CmdList & list
		, PreExecuteActions & preExecuteActions );
}
