/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl4Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl4
{
	void buildScissorCommand( uint32_t firstScissor
		, VkScissorArray scissors
		, CmdList & list );
}
