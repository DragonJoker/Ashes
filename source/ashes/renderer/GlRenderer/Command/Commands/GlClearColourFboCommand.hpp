/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

#include "renderer/GlRenderer/Enum/GlFormat.hpp"

namespace ashes::gl
{
	void buildClearColourFboCommand( VkDevice device
		, ContextStateStack const & stack
		, VkImage image
		, VkImageLayout imageLayout
		, VkClearColorValue value
		, ArrayView< VkImageSubresourceRange const > ranges
		, CmdList & list );
}
