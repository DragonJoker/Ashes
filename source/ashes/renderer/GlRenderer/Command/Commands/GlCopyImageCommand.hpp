/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl
{
	void buildCopyImageCommand( ContextStateStack & stack
		, VkDevice device
		, VkImageCopy copyInfo
		, VkImage srcImage
		, VkImage dstImage
		, CmdList & list
		, VkImageViewArray & views );
}
