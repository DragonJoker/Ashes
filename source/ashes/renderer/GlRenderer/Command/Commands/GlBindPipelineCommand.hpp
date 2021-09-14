/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl
{
	void buildBindPipelineCommand( ContextStateStack & stack
		, VkDevice device
		, VkPipeline pipeline
		, VkPipelineBindPoint bindingPoint
		, CmdList & list
		, PreExecuteActions & preExecuteActions );
	void buildUnbindPipelineCommand( ContextStateStack & stack
		, VkDevice device
		, CmdList & list );
}
