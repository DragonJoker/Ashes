/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl3Renderer/Command/Commands/GlCommandBase.hpp"

#include "renderer/Gl3Renderer/Enum/GlFormat.hpp"

namespace ashes::gl3
{
	void buildClearDepthStencilFboCommand( VkDevice device
		, ContextStateStack const & stack
		, VkImage image
		, VkImageLayout imageLayout
		, VkClearDepthStencilValue value
		, VkImageSubresourceRangeArray ranges
		, CmdList & list );
}
