/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

namespace ashes::gl
{
	void buildBindDescriptorSetCommand( VkDevice device
		, VkDescriptorSet descriptorSet
		, uint32_t descriptorSetIndex
		, VkPipelineLayout pipelineLayout
		, ArrayView< uint32_t const > const & dynamicOffsets
		, uint32_t & dynamicOffsetIndex
		, VkPipelineBindPoint bindingPoint
		, CmdList & list );
}
