/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl
{
	void buildClearAttachmentsCommand( ContextStateStack & stack
		, ArrayView< VkClearAttachment const > clearAttaches
		, ArrayView< VkClearRect const > clearRects
		, CmdList & list
		, PreExecuteActions & preExecuteActions );
}
