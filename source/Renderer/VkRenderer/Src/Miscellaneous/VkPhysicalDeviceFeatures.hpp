/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#ifndef ___VkRenderer_PhysicalDeviceFeatures_HPP___
#define ___VkRenderer_PhysicalDeviceFeatures_HPP___
#pragma once

#include <Miscellaneous/PhysicalDeviceFeatures.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un renderer::PhysicalDeviceFeatures en VkPhysicalDeviceFeatures.
	*\param[in] mode
	*	Le renderer::PhysicalDeviceFeatures.
	*\return
	*	Le VkPhysicalDeviceFeatures.
	*/
	VkPhysicalDeviceFeatures convert( renderer::PhysicalDeviceFeatures const & value );
}

#endif
