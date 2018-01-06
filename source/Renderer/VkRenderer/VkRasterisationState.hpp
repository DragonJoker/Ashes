/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <VkLib/RasterisationState.hpp>
#include <Renderer/RasterisationState.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::RasterisationState en vk::RasterisationState.
	*\param[in] state
	*	Le renderer::RasterisationState.
	*\return
	*	Le vk::RasterisationState.
	*/
	vk::RasterisationState convert( renderer::RasterisationState const & state );
}
