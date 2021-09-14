/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl
{
	void buildDispatchCommand( uint32_t groupCountX
		, uint32_t groupCountY
		, uint32_t groupCountZ
		, CmdList & list );
}
