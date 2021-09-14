/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_DeviceQueueCreateFlags_HPP___
#define ___AshesPP_DeviceQueueCreateFlags_HPP___
#pragma once

namespace ashes
{
#ifdef VK_API_VERSION_1_1
	/**
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*/
	inline std::string getName( VkDeviceQueueCreateFlagBits value )
	{
		switch ( value )
		{
		case VK_DEVICE_QUEUE_CREATE_PROTECTED_BIT:
			return "protected";
		default:
			assert( false && "Unsupported VkDeviceQueueCreateFlagBits." );
			return "Unsupported VkDeviceQueueCreateFlagBits";
		}
	}
#endif
}

#endif
