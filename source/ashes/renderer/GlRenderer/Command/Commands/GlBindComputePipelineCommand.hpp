/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl
{
	void buildBindComputePipelineCommand( ContextStateStack & stack
		, VkPipeline pipeline
		, VkPipelineBindPoint bindingPoint
		, CmdList & list );
	void buildUnbindComputePipelineCommand( ContextStateStack & stack
		, CmdList & list );
}
