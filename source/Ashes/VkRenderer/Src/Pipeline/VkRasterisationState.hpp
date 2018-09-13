/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <Pipeline/RasterisationState.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::RasterisationState en VkPipelineRasterizationStateCreateInfo.
	*\param[in] state
	*	Le ashes::RasterisationState.
	*\return
	*	Le VkPipelineRasterizationStateCreateInfo.
	*/
	VkPipelineRasterizationStateCreateInfo convert( ashes::RasterisationState const & state );
}
