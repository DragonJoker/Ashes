/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <VkLib/VkLibPrerequisites.hpp>

#include <Renderer/ImageMemoryBarrier.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::ImageMemoryBarrier en VkImageMemoryBarrier.
	*\param[in] barrier
	*	Le renderer::ImageMemoryBarrier.
	*\return
	*	Le VkImageMemoryBarrier.
	*/
	VkImageMemoryBarrier convert( renderer::ImageMemoryBarrier const & barrier );
}
