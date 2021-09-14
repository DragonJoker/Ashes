/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

#include "renderer/GlRenderer/Enum/GlFormat.hpp"

namespace ashes::gl
{
	void buildClearDepthStencilCommand( VkDevice device
		, ContextStateStack & stack
		, VkImage image
		, VkImageLayout imageLayout
		, VkClearDepthStencilValue value
		, ArrayView< VkImageSubresourceRange const > ranges
		, CmdList & list );
}
