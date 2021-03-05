/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl
{
	void buildCopyBufferToImageCommand( VkDevice device
		, VkBufferImageCopy copyInfo
		, VkBuffer src
		, VkImage dst
		, CmdList & list );
}
