/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl3Renderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl3
{
	void buildResetQueryPoolCommand( VkQueryPool pool
		, uint32_t firstQuery
		, uint32_t queryCount
		, CmdList & list );
}
