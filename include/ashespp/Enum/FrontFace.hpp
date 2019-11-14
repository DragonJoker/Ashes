/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_FrontFace_HPP___
#define ___AshesPP_FrontFace_HPP___
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
	inline std::string getName( VkFrontFace value )
	{
		switch ( value )
		{
		case VK_FRONT_FACE_COUNTER_CLOCKWISE:
			return "ccw";
		case VK_FRONT_FACE_CLOCKWISE:
			return "cw";
		default:
			assert( false && "Unsupported VkFrontFace." );
			throw std::runtime_error{ "Unsupported VkFrontFace" };
		}

		return 0;
	}
}

#endif
