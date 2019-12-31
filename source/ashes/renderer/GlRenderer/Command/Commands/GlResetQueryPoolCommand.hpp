/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl
{
	void buildResetQueryPoolCommand( VkQueryPool pool
		, uint32_t firstQuery
		, uint32_t queryCount
		, CmdList & list );
}
