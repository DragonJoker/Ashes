/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_QueryControlFlags_HPP___
#define ___AshesPP_QueryControlFlags_HPP___
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
	inline std::string getName( VkQueryControlFlagBits value )
	{
		switch ( value )
		{
		case VK_QUERY_CONTROL_PRECISE_BIT:
			return "precise";
		default:
			assert( false && "Unsupported VkQueryControlFlagBits." );
			return "Unsupported VkQueryControlFlagBits";
		}
	}
}

#endif
