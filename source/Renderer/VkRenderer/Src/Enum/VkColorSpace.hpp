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
	*	Convertit un renderer::ColorSpace en VkColorSpaceKHR.
	*\param[in] value
	*	Le renderer::ColorSpace.
	*\return
	*	Le VkColorSpaceKHR.
	*/
	VkColorSpaceKHR convert( renderer::ColorSpace const & value );
	/**
	*\brief
	*	Convertit un VkColorSpaceKHR en renderer::ColorSpace.
	*\param[in] value
	*	Le VkColorSpaceKHR.
	*\return
	*	Le renderer::ColorSpace.
	*/
	renderer::ColorSpace convertColorSpace( VkColorSpaceKHR const & value );
}
