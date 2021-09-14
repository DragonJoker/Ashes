/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_SurfaceCounterFlags_HPP___
#define ___AshesPP_SurfaceCounterFlags_HPP___
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
	inline std::string getName( VkSurfaceCounterFlagBitsEXT value )
	{
		switch ( value )
		{
		case VK_SURFACE_COUNTER_VBLANK_EXT:
			return "vblank";
		default:
			assert( false && "Unsupported VkSurfaceCounterFlagBitsEXT." );
			return "Unsupported VkSurfaceCounterFlagBitsEXT";
		}
	}
}

#endif
