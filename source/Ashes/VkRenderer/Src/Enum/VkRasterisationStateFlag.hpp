/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::RasterisationStateFlags en VkPipelineRasterizationStateCreateFlags.
	*\param[in] flags
	*	Le ashes::RasterisationStateFlags.
	*\return
	*	Le VkPipelineRasterizationStateCreateFlags.
	*/
	VkPipelineRasterizationStateCreateFlags convert( ashes::RasterisationStateFlags const & flags );
}
