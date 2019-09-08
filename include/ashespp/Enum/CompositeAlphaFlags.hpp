/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_CompositeAlphaFlag_HPP___
#define ___AshesPP_CompositeAlphaFlag_HPP___
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
	inline std::string getName( VkCompositeAlphaFlagBitsKHR value )
	{
		switch ( value )
		{
		case VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR:
			return "opaque";
		case VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR:
			return "pre_multiplied";
		case VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR:
			return "post_multiplied";
		case VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR:
			return "inherit";
		default:
			assert( false && "Unsupported VkCompositeAlphaFlagBitsKHR." );
			throw std::runtime_error{ "Unsupported VkCompositeAlphaFlagBitsKHR" };
		}

		return 0;
	}
}

#endif
