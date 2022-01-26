/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_AccelerationStructureCreateFlagBits_HPP___
#define ___AshesPP_AccelerationStructureCreateFlagBits_HPP___
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
	inline std::string getName( VkAccelerationStructureCreateFlagBitsKHR value )
	{
		switch ( value )
		{
		case VK_ACCELERATION_STRUCTURE_CREATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT_KHR:
			return "device_address_capture_replay";
		case VK_ACCELERATION_STRUCTURE_CREATE_MOTION_BIT_NV:
			return "motion";
		default:
			assert( false && "Unsupported VkAccelerationStructureCreateFlagBitsKHR." );
			return "Unsupported VkAccelerationStructureCreateFlagBitsKHR";
		}
	}
#endif
}

#endif
