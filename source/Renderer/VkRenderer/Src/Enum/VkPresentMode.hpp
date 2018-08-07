/*
This file belongs to RendererLib.
See LICENSE file in root folder
*/
#pragma once

#include <RendererPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::PresentMode en VkPresentModeKHR.
	*\param[in] value
	*	Le renderer::PresentMode.
	*\return
	*	Le VkPresentModeKHR.
	*/
	VkPresentModeKHR convert( renderer::PresentMode const & value );
	/**
	*\brief
	*	Convertit un VkPresentModeKHR en renderer::PresentMode.
	*\param[in] value
	*	Le VkPresentModeKHR.
	*\return
	*	Le renderer::PresentMode.
	*/
	renderer::PresentMode convert( VkPresentModeKHR const & value );
}
