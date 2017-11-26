/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include <VkLib/FlagCombination.hpp>

namespace renderer
{
	/**
	*\brief
	*	Enumération des types d'accès.
	*/
	enum class RasterisationStateFlag
		: uint32_t
	{
	};
	VkLib_ImplementFlag( RasterisationStateFlag )
	/**
	*\brief
	*	Convertit un renderer::RasterisationStateFlags en VkPipelineRasterizationStateCreateFlags.
	*\param[in] flags
	*	Le renderer::RasterisationStateFlags.
	*\return
	*	Le VkPipelineRasterizationStateCreateFlags.
	*/
	VkPipelineRasterizationStateCreateFlags convert( RasterisationStateFlags const & flags );
}
