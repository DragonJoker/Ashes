/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ColorComponentFlag_HPP___
#define ___AshesPP_ColorComponentFlag_HPP___
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
	inline std::string getName( VkColorComponentFlagBits value )
	{
		switch ( value )
		{
		case VK_COLOR_COMPONENT_R_BIT:
			return "r";
		case VK_COLOR_COMPONENT_G_BIT:
			return "g";
		case VK_COLOR_COMPONENT_B_BIT:
			return "b";
		case VK_COLOR_COMPONENT_A_BIT:
			return "a";
		default:
			assert( false && "Unsupported VkColorComponentFlagBits." );
			return "Unsupported VkColorComponentFlagBits";
		}
	}
}

#endif
