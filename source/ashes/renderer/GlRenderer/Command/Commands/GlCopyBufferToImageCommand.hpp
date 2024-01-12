/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl
{
	void buildCopyBufferToImageCommand( ContextStateStack & stack
		, VkDevice device
		, VkBufferImageCopy copyInfo
		, GlFormat unpackFormat
		, GlType unpackType
		, DeviceMemoryBinding const & src
		, VkImage dst
		, CmdList & list );
	void buildCopyBufferToImageCommand( ContextStateStack & stack
		, VkDevice device
		, VkBufferImageCopy const & copyInfo
		, VkBuffer src
		, VkImage dst
		, CmdList & list );
}
