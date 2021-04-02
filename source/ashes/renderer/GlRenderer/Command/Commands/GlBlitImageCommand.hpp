/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

#include "renderer/GlRenderer/Enum/GlAttachmentPoint.hpp"
#include "renderer/GlRenderer/Enum/GlAttachmentType.hpp"
#include "renderer/GlRenderer/Enum/GlFilter.hpp"
#include "renderer/GlRenderer/Enum/GlImageAspectFlag.hpp"

namespace ashes::gl
{
	void buildBlitImageCommand( ContextStateStack & stack
		, VkDevice device
		, VkImage srcImage
		, VkImage dstImage
		, VkImageBlit region
		, VkFilter filter
		, CmdList & list );
}
