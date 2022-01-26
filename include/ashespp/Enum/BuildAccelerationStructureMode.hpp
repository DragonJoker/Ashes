/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_BuildAccelerationStructureMode_HPP___
#define ___AshesPP_BuildAccelerationStructureMode_HPP___
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
	inline std::string getName( VkBuildAccelerationStructureModeKHR value )
	{
		switch ( value )
		{
		case VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR:
			return "build";
		case VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR:
			return "update";
		default:
			assert( false && "Unsupported VkBuildAccelerationStructureModeKHR." );
			return "Unsupported VkBuildAccelerationStructureModeKHR";
		}
	}
#endif
}

#endif
