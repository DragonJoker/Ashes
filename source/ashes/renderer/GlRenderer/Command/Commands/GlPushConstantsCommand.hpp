/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

#include "renderer/GlRenderer/Shader/GlShaderDesc.hpp"

namespace ashes::gl
{
	void buildPushConstantsCommand( VkDevice device
		, VkShaderStageFlags stageFlags
		, VkPipelineLayout pipelineLayout
		, ConstantsLayout const & constants
		, ByteArray const & data
		, CmdList & list );
}
