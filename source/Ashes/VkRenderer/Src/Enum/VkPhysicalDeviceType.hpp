/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include <AshesPrerequisites.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Convertit un ashes::PhysicalDeviceType en VkPhysicalDeviceType.
	*\param[in] value
	*	Le ashes::PhysicalDeviceType.
	*\return
	*	Le VkPhysicalDeviceType.
	*/
	VkPhysicalDeviceType convert( ashes::PhysicalDeviceType const & value );
	/**
	*\brief
	*	Convertit un VkPhysicalDeviceType en ashes::PhysicalDeviceType.
	*\param[in] value
	*	Le VkPhysicalDeviceType.
	*\return
	*	Le ashes::PhysicalDeviceType.
	*/
	ashes::PhysicalDeviceType convertPhysicalDeviceType( VkPhysicalDeviceType const & value );
}
