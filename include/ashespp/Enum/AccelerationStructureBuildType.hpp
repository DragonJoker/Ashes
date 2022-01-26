/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_AccelerationStructureBuildType_HPP___
#define ___AshesPP_AccelerationStructureBuildType_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
#if VK_KHR_acceleration_structure
	/**
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*/
	inline std::string getName( VkAccelerationStructureBuildTypeKHR value )
	{
		switch ( value )
		{
		case VK_ACCELERATION_STRUCTURE_BUILD_TYPE_HOST_KHR:
			return "host";
		case VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR:
			return "device";
		case VK_ACCELERATION_STRUCTURE_BUILD_TYPE_HOST_OR_DEVICE_KHR:
			return "host_or_device";
		default:
			assert( false && "Unsupported VkAccelerationStructureBuildTypeKHR." );
			return "Unsupported VkAccelerationStructureBuildTypeKHR";
		}
	}
#endif
}

#endif
