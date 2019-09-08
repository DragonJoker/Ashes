/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_PhysicalDeviceType_HPP___
#define ___AshesPP_PhysicalDeviceType_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*/
	inline std::string getName( VkPhysicalDeviceType value )
	{
		switch ( value )
		{
		case VK_PHYSICAL_DEVICE_TYPE_OTHER:
			return "other";
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			return "integrated_gpu";
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			return "discrete_gpu";
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			return "virtual_gpu";
		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			return "cpu";
		default:
			assert( false && "Unsupported VkPhysicalDeviceType." );
			throw std::runtime_error{ "Unsupported VkPhysicalDeviceType" };
		}

		return std::string{};
	}
}

#endif
