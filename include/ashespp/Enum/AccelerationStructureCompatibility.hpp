/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_AccelerationStructureCompatibility_HPP___
#define ___AshesPP_AccelerationStructureCompatibility_HPP___
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
	inline std::string getName( VkAccelerationStructureCompatibilityKHR value )
	{
		switch ( value )
		{
		case VK_ACCELERATION_STRUCTURE_COMPATIBILITY_COMPATIBLE_KHR:
			return "compatible";
		case VK_ACCELERATION_STRUCTURE_COMPATIBILITY_INCOMPATIBLE_KHR:
			return "incompatible";
		default:
			assert( false && "Unsupported VkAccelerationStructureCompatibilityKHR." );
			return "Unsupported VkAccelerationStructureCompatibilityKHR";
		}
	}
#endif
}

#endif
