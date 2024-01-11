/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_InstanceCreateFlags_HPP___
#define ___AshesPP_InstanceCreateFlags_HPP___
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
	inline std::string getName( VkInstanceCreateFlagBits value )
	{
		switch ( value )
		{
#if VK_KHR_portability_enumeration
		case VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR:
			return "enumerate_portability";
#endif
		default:
			assert( false && "Unsupported VkInstanceCreateFlagBits." );
			return "Unsupported VkInstanceCreateFlagBits";
		}
	}
	
}

#endif
