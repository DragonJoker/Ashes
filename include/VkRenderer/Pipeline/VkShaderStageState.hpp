/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Ashes/Pipeline/ShaderStageState.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::ShaderStageState en VkPipelineShaderStageCreateInfo.
	*\param[in] state
	*	Le ashes::ShaderStageState.
	*\return
	*	Le VkPipelineShaderStageCreateInfo.
	*/
	VkPipelineShaderStageCreateInfo convert( ashes::ShaderStageState const & state
		, VkSpecializationInfo const * specialisationInfo = nullptr );
}
