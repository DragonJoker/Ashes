/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_BorderColour_HPP___
#define ___AshesPP_BorderColour_HPP___
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
	inline std::string getName( VkBorderColor value )
	{
		switch ( value )
		{
		case VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK:
			return "float_transparent_black";
		case VK_BORDER_COLOR_INT_TRANSPARENT_BLACK:
			return "int_transparent_black";
		case VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK:
			return "float_opaque_black";
		case VK_BORDER_COLOR_INT_OPAQUE_BLACK:
			return "int_opaque_black";
		case VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE:
			return "float_opaque_white";
		case VK_BORDER_COLOR_INT_OPAQUE_WHITE:
			return "int_opaque_white";
		default:
			assert( false && "Unsupported VkBorderColor." );
			return "Unsupported VkBorderColor";
		}
	}
}

#endif
